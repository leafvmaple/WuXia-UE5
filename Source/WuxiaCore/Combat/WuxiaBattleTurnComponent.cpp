// WuxiaBattleTurnComponent.cpp
#include "Combat/WuxiaBattleTurnComponent.h"

UWuxiaBattleTurnComponent::UWuxiaBattleTurnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWuxiaBattleTurnComponent::BeginTurn()
{
	bHasMoved = false;
	bHasActed = false;
}

void UWuxiaBattleTurnComponent::EndTurn()
{
	bHasMoved = true;
	bHasActed = true;
}
