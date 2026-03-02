// WuxiaGameInstance.cpp
#include "Core/WuxiaGameInstance.h"
#include "Character/WuxiaCharacterData.h"
#include "Inventory/WuxiaInventoryComponent.h"

UWuxiaGameInstance::UWuxiaGameInstance()
{
}

void UWuxiaGameInstance::Init()
{
	Super::Init();
	InventoryComponent = NewObject<UWuxiaInventoryComponent>(this, TEXT("Inventory"));
}

void UWuxiaGameInstance::AddPartyMember(UWuxiaCharacterData* CharacterData)
{
	if (CharacterData && !PartyMembers.Contains(CharacterData) && PartyMembers.Num() < 6)
	{
		PartyMembers.Add(CharacterData);
	}
}

void UWuxiaGameInstance::RemovePartyMember(UWuxiaCharacterData* CharacterData)
{
	PartyMembers.Remove(CharacterData);
}

void UWuxiaGameInstance::SetProgressFlag(FGameplayTag Flag, bool bValue)
{
	if (bValue)
	{
		ProgressFlags.AddTag(Flag);
	}
	else
	{
		ProgressFlags.RemoveTag(Flag);
	}
}

bool UWuxiaGameInstance::GetProgressFlag(FGameplayTag Flag) const
{
	return ProgressFlags.HasTag(Flag);
}
