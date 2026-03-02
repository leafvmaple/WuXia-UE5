// WuxiaInventoryWidget.h — 背包/装备 UI
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WuxiaInventoryWidget.generated.h"

class UWuxiaInventoryComponent;

UCLASS()
class WUXIACORE_API UWuxiaInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Wuxia|UI")
	void BindInventory(UWuxiaInventoryComponent* InInventory);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void RefreshInventoryList();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void RefreshEquipmentSlots();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|UI")
	TWeakObjectPtr<UWuxiaInventoryComponent> Inventory;
};
