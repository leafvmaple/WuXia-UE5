// WuxiaInventoryWidget.cpp
#include "UI/Menu/WuxiaInventoryWidget.h"
#include "Inventory/WuxiaInventoryComponent.h"

void UWuxiaInventoryWidget::BindInventory(UWuxiaInventoryComponent* InInventory)
{
	Inventory = InInventory;
}
