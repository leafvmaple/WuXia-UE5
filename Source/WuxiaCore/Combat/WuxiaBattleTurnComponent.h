// WuxiaBattleTurnComponent.h — 回合状态追踪组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaBattleTurnComponent.generated.h"

UCLASS(ClassGroup = (Wuxia), meta = (BlueprintSpawnableComponent))
class WUXIACORE_API UWuxiaBattleTurnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWuxiaBattleTurnComponent();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void BeginTurn();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void EndTurn();

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	bool HasMoved() const { return bHasMoved; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	bool HasActed() const { return bHasActed; }

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void SetMoved(bool bMoved) { bHasMoved = bMoved; }

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void SetActed(bool bActed) { bHasActed = bActed; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	bool IsTurnComplete() const { return bHasMoved && bHasActed; }

protected:
	bool bHasMoved = false;
	bool bHasActed = false;
};
