#include "UT4XKickIdlers.h"


#define DEBUGLOG Verbose

AUT4XKickIdlers::AUT4XKickIdlers(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

	DisplayName = FText::FromString("UT4X KickIdlers 1.1");
	Author = FText::FromString("Thomas 'XtremeXp/Winter' P.");
	Description = FText::FromString("UT4 server mutator for automatically kicking idlers.");
}


void AUT4XKickIdlers::Init_Implementation(const FString& Options)
{

	AUTGameMode* GM = Cast<AUTGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {
		// auto save config at start so UT4X.ini file will appear in <Linux/Windows>Server/UnrealTournament/Saved/Config/<Linux/Windows>Server
		// making easier for admins to modify it afterwards
		SaveConfig();

		// global setting
		bool checkIdlers = KickIdlersEnabled;

		// private games settings
		if ((GM->bIsLANGame || GM->bPrivateMatch || !GM->ServerPassword.IsEmpty()) && !KickIdlersInPrivateGamesEnabled) {
			checkIdlers = false;
		}

		// command line option
		checkIdlers &= (UGameplayStatics::GetIntOption(Options, TEXT("4XKickIdlersEnabled"), 1) == 1) ? true : false;

		if (checkIdlers) {
			// check idling players each 5 seconds
			GetWorld()->GetTimerManager().SetTimer(CheckPlayerIdlingTimerHandle, this, &AUT4XKickIdlers::CheckPlayersIdling, 5.f, true);

			UE_LOG(UT4X, Log, TEXT("Auto-kick idlers v1.1 is enabled with MaxIdlingDuration = %is and MinPlayersForKickIdlers >= %i."), MaxIdlingDuration, MinPlayersForKickIdlers);
		}
		else {
			UE_LOG(UT4X, Log, TEXT("Auto-kick idlers v1.1 is disabled for this game (private game or globally disabled)."));
		}
	}

	Super::Init_Implementation(Options);
}

void AUT4XKickIdlers::Mutate_Implementation(const FString& MutateString, APlayerController* Sender)
{

	TArray<FString> Tokens;
	AUTPlayerController* UTSender = Cast<AUTPlayerController>(Sender);


	if (UTSender) {
		AUTPlayerState* UTPS = Cast<AUTPlayerState>(Sender->PlayerState);

		// allows as an admin to enable / disable auto kick mutator
		// note if there are multiple instance within hub, command will have to be executed within each instance (to disable)
		if (UTPS && UTPS->bIsRconAdmin) {
			if (MutateString == "disablekickidlers") {
				KickIdlersEnabled = false;
				SaveConfig();

				// stop auto kick idler timer if any active
				GetWorldTimerManager().ClearTimer(CheckPlayerIdlingTimerHandle);

				UTSender->ClientSay(UTPS, TEXT("KickIdlers has been disabled."), ChatDestinations::System);
			}
			else if (MutateString == "enablekickidlers") {
				KickIdlersEnabled = true;
				SaveConfig();

				// don't auto add timer, will be added or not at next round
				UTSender->ClientSay(UTPS, TEXT("KickIdlers has been enabled."), ChatDestinations::System);
			}
		}
	}

	Super::Mutate_Implementation(MutateString, Sender);
}

void AUT4XKickIdlers::ModifyPlayer_Implementation(APawn* Other, bool bIsNewSpawn)
{

	// some custom gamemodes does not handle match state name change properly
	// as such once player spawn reset idling time
	// this will prevent player to be kicked straight after spawning after dying before and spectating for some time
	// until other players has died (e.g: elimination gametype)
	if (Other) {
		AUTPlayerController* PC = Cast<AUTPlayerController>(Other->GetController());

		if (PC) {

			AUTPlayerState* UTPS = Cast<AUTPlayerState>(PC->PlayerState);

			if (UTPS && !UTPS->bIsABot) {
				UTPS->NotIdle();
			}
		}
	}

	Super::ModifyPlayer_Implementation(Other, bIsNewSpawn);
}

void AUT4XKickIdlers::NotifyMatchStateChange_Implementation(FName NewState)
{
	// no need check idling players at match end
	if (NewState == MatchState::WaitingPostMatch || NewState == MatchState::MapVoteHappening) {
		GetWorldTimerManager().ClearTimer(CheckPlayerIdlingTimerHandle);
	}

	Super::NotifyMatchStateChange_Implementation(NewState);
}


// executed each 5 seconds, must be quick !
void AUT4XKickIdlers::CheckPlayersIdling() {

	AUTGameMode* GM = Cast<AUTGameMode>(GetWorld()->GetAuthGameMode());

	// only check idlers during in progress match state
	if (GM && GM->GetMatchState() == MatchState::InProgress) {

		AUTGameState* UTGameState = GM->UTGameState;

		// no kick for lan games and if there are less than 2 players
		if (UTGameState && GM->GameSession && GM->GetNumPlayers() >= MinPlayersForKickIdlers)
		{
			for (int32 i = 0; i < UTGameState->PlayerArray.Num(); i++)
			{

				AUTPlayerState* UTPS = Cast<AUTPlayerState>(UTGameState->PlayerArray[i]);

				if (UTPS) {

					// do not kick bot, spectators, out of lives players (e.g: blitz) and player who just died
					// null character means player has just died and is in spectate mode
					bool IsPlayerIdling = UTPS && !UTPS->bIsABot && !UTPS->bOnlySpectator && !UTPS->bIsSpectator && !UTPS->bOutOfLives && !UTPS->bIsInactive && !UTPS->bIsWarmingUp && UTPS->GetUTCharacter() != nullptr;

					if (!IsPlayerIdling) {
						continue;
					}
					
					// kick idling player if it has reached max inactive time
					int IdlingDuration = GetWorld()->GetTimeSeconds() - UTPS->LastActiveTime;

					bool KickPlayerIdling = IsPlayerIdling && (IdlingDuration >= MaxIdlingDuration);

					if (KickPlayerIdling && Cast<APlayerController>(UTPS->GetOwner()))
					{
						AUTPlayerController* Controller = Cast<AUTPlayerController>(UTPS->GetOwner());

						// FIXME a very few times player may have crash and kick message is displayed over and over multiple times
						if (Controller)
						{
							FString PublicKickMsg = FString::Printf(TEXT("%s has been automatically kicked. (AFK >= %i s.)"), *UTPS->PlayerName, MaxIdlingDuration);

							if (GM->IsGameInstanceServer())
							{
								// TODO custom kick message if possible
								Controller->ClientReturnToLobby(true, true);
								BroadcastMessageToPlayers(PublicKickMsg, ChatDestinations::System);
							}
							else if (GM->GameSession != nullptr)
							{
								if (GM->GameSession->KickPlayer(Controller, NSLOCTEXT("General", "IdleKick", "You were kicked for being idle."))) {
									BroadcastMessageToPlayers(PublicKickMsg, ChatDestinations::System);
								}
							}
						}
					}
				}
			}
		}
	}
}

void AUT4XKickIdlers::BroadcastMessageToPlayers(FString& Msg, FName ChatDestination) {

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AUTPlayerController* NextPlayer = Cast<AUTPlayerController>(*Iterator);

		if (NextPlayer && NextPlayer->UTPlayerState && !NextPlayer->PlayerState->bIsABot)
		{
			NextPlayer->ClientSay(nullptr, Msg, ChatDestination);
		}
	}
}
