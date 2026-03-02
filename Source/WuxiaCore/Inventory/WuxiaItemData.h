// WuxiaItemData.h — 物品数据资产
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaItemData.generated.h"

class UGameplayEffect;
class UTexture2D;

UCLASS(BlueprintType)
class WUXIACORE_API UWuxiaItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ---- 基础信息 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	EWuxiaItemType ItemType = EWuxiaItemType::Consumable;

	// ---- 装备属性（武器/防具） ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (EditCondition = "ItemType == EWuxiaItemType::Weapon || ItemType == EWuxiaItemType::Armor"))
	EWuxiaEquipSlot EquipSlot = EWuxiaEquipSlot::Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备")
	float AttackBonus = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备")
	float DefenseBonus = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备")
	float AgilityBonus = 0.f;

	// ---- 使用效果（消耗品） ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "效果")
	TSubclassOf<UGameplayEffect> UseEffect;

	// ---- 购买/出售价格 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "经济")
	int32 BuyPrice = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "经济")
	int32 SellPrice = 50;

	// ---- 堆叠 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	int32 MaxStackSize = 99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	bool bIsConsumable = false;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("WuxiaItem", GetFName());
	}
};
