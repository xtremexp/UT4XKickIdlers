#pragma once

#include "Core.h"
#include "UnrealTournament.h"
#include "UTMutator.h"

#include "UT4XKickIdlers.generated.h"

DEFINE_LOG_CATEGORY_STATIC(UT4X, Log, All);


UCLASS(Config = Mod)
class AUT4XKickIdlers : public AUTMutator
{

	GENERATED_UCLASS_BODY()

	/*
	* If true kick idlers is enabled else false
	*/
	UPROPERTY(Config = Mod)
	bool KickIdlersEnabled = true;

	/*
	* Whether kick idlers is enabled for private games (passworded, lan, ...)
	*/
	UPROPERTY(Config = Mod)
	bool KickIdlersInPrivateGamesEnabled = false;

	/*
	* Max idling duration until player is kicked
	*/
	UPROPERTY(Config = Mod)
	int MaxIdlingDuration = 90;

	/*
	* Will only kick if at least >= X (default 2) players in server
	*/
	UPROPERTY(Config = Mod)
	int MinPlayersForKickIdlers = 2;

	/*
	* Override init mutator function to start checking idling players
	*/
	void Init_Implementation(const FString& Options) override;


private:

	/*
	* Check and kick idling players
	*/
	void CheckPlayersIdling();

	void BroadcastMessageToPlayers(FString& Msg, FName ChatDestination);

	FTimerHandle CheckPlayerIdlingTimerHandle;
};