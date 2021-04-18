#pragma once

#include "Core.h"
#include "UnrealTournament.h"
#include "UTMutator.h"
#include "UTWorldSettings.h"
#include "UTGameEngine.h"

#include "UT4XKickIdlers.generated.h"

DEFINE_LOG_CATEGORY_STATIC(UT4X, Log, All);


UCLASS(Config = UT4X)
class AUT4XKickIdlers : public AUTMutator
{

	GENERATED_UCLASS_BODY()

	/*
	* If true kick idlers is enabled else false
	*/
	UPROPERTY(Config = UT4X)
	bool KickIdlersEnabled = true;

	/*
	* Whether kick idlers is enabled for private games (passworded, lan, ...)
	*/
	UPROPERTY(Config = UT4X)
	bool KickIdlersInPrivateGamesEnabled = false;

	/*
	* Max idling duration until player is kicked
	*/
	UPROPERTY(Config = UT4X)
	int MaxIdlingDuration = 90;

	/*
	* Will only kick if at least >= X (default 2) players in server
	*/
	UPROPERTY(Config = UT4X)
	int MinPlayersForKickIdlers = 2;

	/*
	* Override init mutator function to start checking idling players
	*/
	void Init_Implementation(const FString& Options) override;

	/*
	* Adds some custom "mutate xxx" command
	*/
	void Mutate_Implementation(const FString& MutateString, APlayerController* Sender) override;


private:

	/*
	* Check and kick idling players
	*/
	void CheckPlayersIdling();

	void BroadcastMessageToPlayers(FString& Msg, FName ChatDestination);

	FTimerHandle CheckPlayerIdlingTimerHandle;
};