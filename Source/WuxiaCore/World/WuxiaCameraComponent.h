// WuxiaCameraComponent.h — 等轴测（HD2D）摄像机组件
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "WuxiaCameraComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup = (Wuxia), meta = (BlueprintSpawnableComponent))
class WUXIACORE_API UWuxiaCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UWuxiaCameraComponent();

	// ---- 配置 ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Camera")
	float IsometricPitch = -45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Camera")
	float IsometricYaw = -45.f;

	/** true = 正交投影（纯 HD2D），false = 低 FOV 透视 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Camera")
	bool bUseOrthographic = false;

	/** 正交宽度，仅 bUseOrthographic 时生效，通过父类 SetOrthoWidth 设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Camera", meta = (EditCondition = "bUseOrthographic"))
	float DefaultOrthoWidth = 2048.f;

	/** 透视 FOV，仅 !bUseOrthographic 时生效，通过父类 SetFieldOfView 设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Camera", meta = (EditCondition = "!bUseOrthographic"))
	float DefaultPerspectiveFOV = 30.f;

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Camera")
	void ApplyIsometricSettings();
};
