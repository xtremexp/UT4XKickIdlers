
#include "UT4XKickIdlers.h"

#include "ModuleManager.h"
#include "ModuleInterface.h"

class FUT4XKickIdlersPlugin : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	AUT4XKickIdlers* _UT4XKickIdlers;
};

IMPLEMENT_MODULE( FUT4XKickIdlersPlugin, UT4XKickIdlers )

//================================================
// Startup
//================================================

void FUT4XKickIdlersPlugin::StartupModule()
{
	UE_LOG(UT4X, Log, TEXT("StartupModule UT4X KickIdlers"))
	FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FUT4XKickIdlersPlugin::OnWorldInitialized);
}

//================================================
// Shutdown
//================================================

void FUT4XKickIdlersPlugin::ShutdownModule()
{
	UE_LOG(UT4X, Log, TEXT("ShutdownModule UT4X KickIdlers"))

	if (_UT4XKickIdlers) {
		// nothing to do
	}
}

void FUT4XKickIdlersPlugin::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (World)
	{

		if (_UT4XKickIdlers == NULL) {
			_UT4XKickIdlers = World->SpawnActor<AUT4XKickIdlers>();
		}
	}
}
