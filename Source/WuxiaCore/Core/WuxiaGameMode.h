// WuxiaGameMode.h — 全局游戏模式，管理探索/战斗/对话状态切换
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaGameMode.generated.h"

class AWuxiaBattleManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EWuxiaGamePhase, NewPhase);

UCLASS()
class WUXIACORE_API AWuxiaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWuxiaGameMode();

	// ---- 游戏阶段管理 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|GameMode")
	void SetGamePhase(EWuxiaGamePhase NewPhase);

	UFUNCTION(BlueprintPure, Category = "Wuxia|GameMode")
	EWuxiaGamePhase GetGamePhase() const { return CurrentPhase; }

	// ---- 战斗入口 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|GameMode")
	void StartBattle(TSubclassOf<AWuxiaBattleManager> BattleManagerClass);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|GameMode")
	void EndBattle(bool bVictory);

	UPROPERTY(BlueprintAssignable) FOnGamePhaseChanged OnGamePhaseChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|GameMode")
	EWuxiaGamePhase CurrentPhase = EWuxiaGamePhase::Exploration;

	UPROPERTY(Transient) TObjectPtr<AWuxiaBattleManager> ActiveBattleManager;
};
