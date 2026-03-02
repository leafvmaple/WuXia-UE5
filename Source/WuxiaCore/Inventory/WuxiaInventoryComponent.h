// WuxiaInventoryComponent.h — 背包管理组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaInventoryComponent.generated.h"

class UWuxiaItemData;

USTRUCT(BlueprintType)
struct FWuxiaInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWuxiaItemData> ItemData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;

	bool IsEmpty() const { return ItemData == nullptr || Quantity <= 0; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS(ClassGroup = (Wuxia), meta = (BlueprintSpawnableComponent))
class WUXIACORE_API UWuxiaInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWuxiaInventoryComponent();

	// ---- 物品操作 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Inventory")
	bool AddItem(UWuxiaItemData* Item, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Inventory")
	bool RemoveItem(UWuxiaItemData* Item, int32 Quantity = 1);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Inventory")
	int32 GetItemCount(UWuxiaItemData* Item) const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Inventory")
	bool HasItem(UWuxiaItemData* Item, int32 Quantity = 1) const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Inventory")
	const TArray<FWuxiaInventorySlot>& GetAllItems() const { return Items; }

	// ---- 装备 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Inventory")
	bool EquipItem(UWuxiaItemData* Item, EWuxiaEquipSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Inventory")
	UWuxiaItemData* UnequipItem(EWuxiaEquipSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Inventory")
	UWuxiaItemData* GetEquippedItem(EWuxiaEquipSlot Slot) const;

	// ---- 事件 ----
	UPROPERTY(BlueprintAssignable) FOnInventoryChanged OnInventoryChanged;

protected:
	UPROPERTY()
	TArray<FWuxiaInventorySlot> Items;

	UPROPERTY()
	TMap<EWuxiaEquipSlot, TObjectPtr<UWuxiaItemData>> EquippedItems;
};
