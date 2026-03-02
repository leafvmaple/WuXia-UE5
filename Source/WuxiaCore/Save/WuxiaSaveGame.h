// WuxiaSaveGame.h — 存档系统
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "Inventory/WuxiaInventoryComponent.h"
#include "WuxiaSaveGame.generated.h"

/** 单个角色的存档数据 */
USTRUCT(BlueprintType)
struct FWuxiaCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FPrimaryAssetId CharacterDataId;

	UPROPERTY(BlueprintReadWrite) float Health = 0.f;
	UPROPERTY(BlueprintReadWrite) float MaxHealth = 0.f;
	UPROPERTY(BlueprintReadWrite) float InternalEnergy = 0.f;
	UPROPERTY(BlueprintReadWrite) float Attack = 0.f;
	UPROPERTY(BlueprintReadWrite) float Defense = 0.f;
	UPROPERTY(BlueprintReadWrite) float Agility = 0.f;
	UPROPERTY(BlueprintReadWrite) float Aptitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	TMap<FPrimaryAssetId, float> AbilityProficiencies;

	UPROPERTY(BlueprintReadWrite)
	TMap<EWuxiaEquipSlot, FPrimaryAssetId> Equipment;
};

UCLASS()
class WUXIACORE_API UWuxiaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWuxiaSaveGame();

	// ---- 存档元数据 ----
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FDateTime SaveDateTime;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	float PlayTimeSeconds = 0.f;

	// ---- 队伍 ----
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	TArray<FWuxiaCharacterSaveData> PartyData;

	// ---- 背包 ----
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	TArray<FWuxiaInventorySlot> InventoryData;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	int32 Gold = 0;

	// ---- 位置 ----
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FName CurrentLevelName;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FRotator PlayerRotation = FRotator::ZeroRotator;

	// ---- 进度标志 ----
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FGameplayTagContainer ProgressFlags;

	// ---- 静态工具方法 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Save", meta = (WorldContext = "WorldContextObject"))
	static bool SaveToSlot(UObject* WorldContextObject, const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Save")
	static UWuxiaSaveGame* LoadFromSlot(const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Save")
	static bool DoesSaveExist(const FString& SlotName, int32 UserIndex = 0);
};
