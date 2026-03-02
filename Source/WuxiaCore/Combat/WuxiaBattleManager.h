// WuxiaBattleManager.h — 回合制战斗核心控制器
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaBattleManager.generated.h"

class AWuxiaBattleGrid;
class AWuxiaCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, EBattleState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnStarted, AWuxiaCharacterBase*, ActiveCharacter);

UCLASS()
class WUXIACORE_API AWuxiaBattleManager : public AActor
{
	GENERATED_BODY()

public:
	AWuxiaBattleManager();

	// ---- 战斗流程 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void InitBattle();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void StartNextTurn();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void ExecuteAction(EBattleActionType Action, FIntPoint TargetCell);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void EndCurrentTurn();

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	EBattleState GetBattleState() const { return CurrentState; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	AWuxiaCharacterBase* GetActiveCharacter() const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	AWuxiaBattleGrid* GetBattleGrid() const { return BattleGrid; }

	// ---- 参战角色 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void AddCombatant(AWuxiaCharacterBase* Character, FIntPoint SpawnCell);

	// ---- 事件 ----
	UPROPERTY(BlueprintAssignable) FOnBattleStateChanged OnBattleStateChanged;
	UPROPERTY(BlueprintAssignable) FOnTurnStarted OnTurnStarted;

	// ---- 配置 ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Battle")
	TSubclassOf<AWuxiaBattleGrid> BattleGridClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|Battle")
	int32 GridWidth = 16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|Battle")
	int32 GridHeight = 12;

protected:
	virtual void BeginPlay() override;

	void SetBattleState(EBattleState NewState);
	void GenerateTurnOrder();
	EBattleState CheckBattleResult() const;
	void ProcessAITurn();

	UPROPERTY() TObjectPtr<AWuxiaBattleGrid> BattleGrid;
	UPROPERTY() TArray<TObjectPtr<AWuxiaCharacterBase>> Combatants;
	UPROPERTY() TArray<TObjectPtr<AWuxiaCharacterBase>> TurnOrder;

	EBattleState CurrentState = EBattleState::None;
	int32 CurrentTurnIndex = 0;
};
