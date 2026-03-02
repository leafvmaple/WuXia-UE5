// WuxiaGameMode.cpp
#include "Core/WuxiaGameMode.h"
#include "Core/WuxiaPlayerController.h"
#include "Core/WuxiaHUD.h"
#include "World/WuxiaExplorationPawn.h"
#include "Combat/WuxiaBattleManager.h"

AWuxiaGameMode::AWuxiaGameMode()
{
	PlayerControllerClass = AWuxiaPlayerController::StaticClass();
	DefaultPawnClass = AWuxiaExplorationPawn::StaticClass();
	HUDClass = AWuxiaHUD::StaticClass();
}

void AWuxiaGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetGamePhase(EWuxiaGamePhase::Exploration);
}

void AWuxiaGameMode::SetGamePhase(EWuxiaGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase) return;
	CurrentPhase = NewPhase;
	OnGamePhaseChanged.Broadcast(NewPhase);
}

void AWuxiaGameMode::StartBattle(TSubclassOf<AWuxiaBattleManager> BattleManagerClass)
{
	if (!BattleManagerClass) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActiveBattleManager = GetWorld()->SpawnActor<AWuxiaBattleManager>(BattleManagerClass, FTransform::Identity, Params);

	if (ActiveBattleManager)
	{
		SetGamePhase(EWuxiaGamePhase::Battle);
		ActiveBattleManager->InitBattle();
	}
}

void AWuxiaGameMode::EndBattle(bool bVictory)
{
	if (ActiveBattleManager)
	{
		ActiveBattleManager->Destroy();
		ActiveBattleManager = nullptr;
	}
	SetGamePhase(EWuxiaGamePhase::Exploration);
}
