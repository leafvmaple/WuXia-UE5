// WuxiaGameInstance.h — 跨关卡持久化数据（队伍、背包、进度）
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "WuxiaGameInstance.generated.h"

class UWuxiaCharacterData;
class UWuxiaInventoryComponent;

UCLASS()
class WUXIACORE_API UWuxiaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWuxiaGameInstance();

	// ---- 队伍管理 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Party")
	void AddPartyMember(UWuxiaCharacterData* CharacterData);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Party")
	void RemovePartyMember(UWuxiaCharacterData* CharacterData);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Party")
	const TArray<UWuxiaCharacterData*>& GetPartyMembers() const { return PartyMembers; }

	// ---- 进度标志 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Progress")
	void SetProgressFlag(FGameplayTag Flag, bool bValue);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Progress")
	bool GetProgressFlag(FGameplayTag Flag) const;

	// ---- 背包 ----
	UFUNCTION(BlueprintPure, Category = "Wuxia|Inventory")
	UWuxiaInventoryComponent* GetInventory() const { return InventoryComponent; }

	// ---- 金钱 ----
	UPROPERTY(BlueprintReadWrite, Category = "Wuxia|Inventory")
	int32 Gold = 0;

protected:
	virtual void Init() override;

	UPROPERTY()
	TArray<TObjectPtr<UWuxiaCharacterData>> PartyMembers;

	UPROPERTY()
	FGameplayTagContainer ProgressFlags;

	UPROPERTY()
	TObjectPtr<UWuxiaInventoryComponent> InventoryComponent;
};
