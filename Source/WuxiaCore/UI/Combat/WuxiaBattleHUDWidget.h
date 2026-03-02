// WuxiaBattleHUDWidget.h — 战斗主界面 Widget
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaBattleHUDWidget.generated.h"

class AWuxiaBattleManager;
class AWuxiaCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBattleActionSelected, EBattleActionType, Action, FIntPoint, TargetCell);

UCLASS()
class WUXIACORE_API UWuxiaBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ---- 绑定战斗管理器 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|UI")
	void BindBattleManager(AWuxiaBattleManager* InManager);

	// ---- 显示行动菜单 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowActionMenu(AWuxiaCharacterBase* ActiveCharacter);

	// ---- 隐藏行动菜单 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void HideActionMenu();

	// ---- 更新角色状态 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void UpdateCharacterStatus(AWuxiaCharacterBase* Character);

	// ---- 显示伤害数字 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowDamageNumber(FVector WorldPos, float Damage, bool bIsCritical);

	// ---- 显示战斗结果 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowBattleResult(EBattleState Result);

	// ---- 更新回合顺序 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void UpdateTurnOrder(const TArray<AWuxiaCharacterBase*>& TurnOrder, int32 CurrentIndex);

	// ---- 事件 ----
	UPROPERTY(BlueprintAssignable) FOnBattleActionSelected OnBattleActionSelected;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|UI")
	TWeakObjectPtr<AWuxiaBattleManager> BattleManager;
};
