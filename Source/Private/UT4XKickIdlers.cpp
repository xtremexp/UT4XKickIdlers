#include "UT4XKickIdlers.h"


#define DEBUGLOG Verbose

AUT4XKickIdlers::AUT4XKickIdlers(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

	DisplayName = FText::FromString("UT4X KickIdlers 1.0");
	Author = FText::FromString("Thomas 'XtremeXp/Winter' P.");
	Description = FText::FromString("UT4 server mutator for automatically kicking idlers.");
}


void AUT4XKickIdlers::Init_Implementation(const FString& Options)
{

	AUTGameMode* GM = Cast<AUTGameMode>(GetWorld()->GetAuthGameMode());

	if (GM) {

		// global setting
		bool checkIdlers = KickIdlersEnabled;

		// private games settings
		if ((GM->bIsLANGame || !GM->ServerPassword.IsEmpty()) && !KickIdlersInPrivateGamesEnabled) {
			checkIdlers = false;
		}

		// command line option
		checkIdlers &= (UGameplayStatics::GetIntOption(Options, TEXT("4XKickIdlersEnabled"), 1) == 1) ? true : false;

		if (checkIdlers) {
			// check idling players each 5 seconds
			GetWorld()->GetTimerManager().SetTimer(CheckPlayerIdlingTimerHandle, this, &AUT4XKickIdlers::CheckPlayersIdling, 5.f, true);
			UE_LOG(UT4X, Log, TEXT("Auto-kick idlers is enabled."));
		}
	}

	Super::Init_Implementation(Options);
}


// executed each 5 seconds, must be quick !
void AUT4XKickIdlers::CheckPlayersIdling() {

	AUTGameMode* GM = Cast<AUTGameMode>(GetWorld()->GetAuthGameMode());

	// only check idlers during in progress match state
	if (GM && GM->GetMatchState() == MatchState::InProgress) {

		AUTGameState* UTGameState = GM->UTGameState;

		// no kick for lan games and if there are less than 2 players
		if (UTGameState && GM->GameSession && !GM->bIsLANGame && GM->GetNumPlayers() >= MinPlayersForKickIdlers)
		{
			for (int32 i = 0; i < UTGameState->PlayerArray.Num(); i++)
			{
				AUTPlayerState* UTPS = Cast<AUTPlayerState>(UTGameState->PlayerArray[i]);

				if (UTPS) {

					// do not kick bot, spectators and out of lives players
					bool IsPlayerIdling = UTPS && !UTPS->bIsABot && !UTPS->bOnlySpectator && !UTPS->bOutOfLives && !UTPS->bIsInactive;

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
