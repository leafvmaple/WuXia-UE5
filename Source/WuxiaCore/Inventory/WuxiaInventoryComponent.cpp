// WuxiaInventoryComponent.cpp
#include "Inventory/WuxiaInventoryComponent.h"
#include "Inventory/WuxiaItemData.h"

UWuxiaInventoryComponent::UWuxiaInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UWuxiaInventoryComponent::AddItem(UWuxiaItemData* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0) return false;

	// 尝试堆叠
	for (FWuxiaInventorySlot& Slot : Items)
	{
		if (Slot.ItemData == Item && Slot.Quantity < Item->MaxStackSize)
		{
			int32 CanAdd = FMath::Min(Quantity, Item->MaxStackSize - Slot.Quantity);
			Slot.Quantity += CanAdd;
			Quantity -= CanAdd;

			if (Quantity <= 0)
			{
				OnInventoryChanged.Broadcast();
				return true;
			}
		}
	}

	// 新增槽位
	while (Quantity > 0)
	{
		FWuxiaInventorySlot NewSlot;
		NewSlot.ItemData = Item;
		NewSlot.Quantity = FMath::Min(Quantity, Item->MaxStackSize);
		Items.Add(NewSlot);
		Quantity -= NewSlot.Quantity;
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UWuxiaInventoryComponent::RemoveItem(UWuxiaItemData* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0) return false;
	if (!HasItem(Item, Quantity)) return false;

	int32 Remaining = Quantity;
	for (int32 i = Items.Num() - 1; i >= 0 && Remaining > 0; --i)
	{
		if (Items[i].ItemData == Item)
		{
			int32 ToRemove = FMath::Min(Remaining, Items[i].Quantity);
			Items[i].Quantity -= ToRemove;
			Remaining -= ToRemove;

			if (Items[i].Quantity <= 0)
			{
				Items.RemoveAt(i);
			}
		}
	}

	OnInventoryChanged.Broadcast();
	return true;
}

int32 UWuxiaInventoryComponent::GetItemCount(UWuxiaItemData* Item) const
{
	int32 Total = 0;
	for (const FWuxiaInventorySlot& Slot : Items)
	{
		if (Slot.ItemData == Item)
		{
			Total += Slot.Quantity;
		}
	}
	return Total;
}

bool UWuxiaInventoryComponent::HasItem(UWuxiaItemData* Item, int32 Quantity) const
{
	return GetItemCount(Item) >= Quantity;
}

bool UWuxiaInventoryComponent::EquipItem(UWuxiaItemData* Item, EWuxiaEquipSlot Slot)
{
	if (!Item) return false;

	// 先卸下当前装备
	UnequipItem(Slot);

	EquippedItems.Add(Slot, Item);
	RemoveItem(Item, 1);
	OnInventoryChanged.Broadcast();
	return true;
}

UWuxiaItemData* UWuxiaInventoryComponent::UnequipItem(EWuxiaEquipSlot Slot)
{
	if (TObjectPtr<UWuxiaItemData>* Found = EquippedItems.Find(Slot))
	{
		UWuxiaItemData* OldItem = *Found;
		EquippedItems.Remove(Slot);
		if (OldItem)
		{
			AddItem(OldItem, 1);
		}
		OnInventoryChanged.Broadcast();
		return OldItem;
	}
	return nullptr;
}

UWuxiaItemData* UWuxiaInventoryComponent::GetEquippedItem(EWuxiaEquipSlot Slot) const
{
	if (const TObjectPtr<UWuxiaItemData>* Found = EquippedItems.Find(Slot))
	{
		return *Found;
	}
	return nullptr;
}
