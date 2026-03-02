// WuxiaExplorationPawn.h — 探索模式可操控角色（含摄像机弹簧臂）
#pragma once

#include "CoreMinimal.h"
#include "Character/WuxiaCharacterBase.h"
#include "WuxiaExplorationPawn.generated.h"

class USpringArmComponent;
class UWuxiaCameraComponent;

UCLASS()
class WUXIACORE_API AWuxiaExplorationPawn : public AWuxiaCharacterBase
{
	GENERATED_BODY()

public:
	AWuxiaExplorationPawn();

	UFUNCTION(BlueprintPure, Category = "Wuxia|Camera")
	USpringArmComponent* GetSpringArm() const { return SpringArm; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Camera")
	UWuxiaCameraComponent* GetIsometricCamera() const { return IsometricCamera; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|Camera")
	TObjectPtr<UWuxiaCameraComponent> IsometricCamera;
};
