// WuxiaPlayerController.h — 玩家控制器，管理输入模式切换与摄像机
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class WUXIACORE_API AWuxiaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWuxiaPlayerController();

	// ---- 输入模式切换 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Input")
	void SwitchInputForPhase(EWuxiaGamePhase Phase);

	// ---- 摄像机 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Camera")
	void SetupIsometricCamera();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** 如果编辑器中未配置输入资产，运行时自动创建默认版本 */
	void CreateDefaultInputAssets();

	// ---- 探索模式输入 ----
	void HandleMoveInput(const struct FInputActionValue& Value);
	void HandleInteractInput();

	// ---- Enhanced Input ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Input")
	TObjectPtr<UInputMappingContext> ExplorationMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Input")
	TObjectPtr<UInputMappingContext> BattleMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Input")
	TObjectPtr<UInputAction> InteractAction;

	// ---- 摄像机参数 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wuxia|Camera")
	float CameraPitch = -45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wuxia|Camera")
	float CameraDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wuxia|Camera")
	float CameraInterpSpeed = 5.0f;
};
