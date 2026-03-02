// WuxiaSaveGame.cpp
#include "Save/WuxiaSaveGame.h"
#include "Core/WuxiaGameInstance.h"
#include "Character/WuxiaCharacterData.h"
#include "Character/WuxiaAttributeSet.h"
#include "Kismet/GameplayStatics.h"

UWuxiaSaveGame::UWuxiaSaveGame()
{
}

bool UWuxiaSaveGame::SaveToSlot(UObject* WorldContextObject, const FString& SlotName, int32 UserIndex)
{
	if (!WorldContextObject) return false;

	UWuxiaSaveGame* SaveGameObj = Cast<UWuxiaSaveGame>(UGameplayStatics::CreateSaveGameObject(UWuxiaSaveGame::StaticClass()));
	if (!SaveGameObj) return false;

	SaveGameObj->SaveSlotName = SlotName;
	SaveGameObj->SaveDateTime = FDateTime::Now();

	// 从 GameInstance 收集数据
	UWuxiaGameInstance* GI = Cast<UWuxiaGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (GI)
	{
		SaveGameObj->Gold = GI->Gold;

		// 保存背包
		if (GI->GetInventory())
		{
			SaveGameObj->InventoryData = GI->GetInventory()->GetAllItems();
		}
	}

	// 保存玩家位置
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC && PC->GetPawn())
	{
		SaveGameObj->PlayerLocation = PC->GetPawn()->GetActorLocation();
		SaveGameObj->PlayerRotation = PC->GetPawn()->GetActorRotation();
	}

	SaveGameObj->CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(WorldContextObject);

	return UGameplayStatics::SaveGameToSlot(SaveGameObj, SlotName, UserIndex);
}

UWuxiaSaveGame* UWuxiaSaveGame::LoadFromSlot(const FString& SlotName, int32 UserIndex)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		return nullptr;
	}

	return Cast<UWuxiaSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
}

bool UWuxiaSaveGame::DoesSaveExist(const FString& SlotName, int32 UserIndex)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}
