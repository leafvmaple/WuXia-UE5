// WuxiaSpriteComponent.h — HD2D 精灵 Billboard 组件（ProceduralMesh 版）
#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaSpriteComponent.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;

UCLASS(ClassGroup = (Wuxia), meta = (BlueprintSpawnableComponent))
class WUXIACORE_API UWuxiaSpriteComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UWuxiaSpriteComponent();

	// ---- 精灵设置 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void SetSpriteTexture(UTexture2D* InTexture);

	/** 设置基础材质（Unlit + 透明），通常在编辑器或初始化时调用 */
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void SetSpriteMaterial(UMaterialInterface* InMaterial);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void SetDirection(EWuxiaSpriteDirection NewDirection);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void SetAnimState(EWuxiaSpriteAnimState NewState);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void PlayFlipbook(float InPlayRate = 8.f);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void StopFlipbook();

	// ---- 平面尺寸 ----
	/** 精灵平面的宽度（UU，默认 100 = 1 米） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Sprite")
	float PlaneWidth = 100.f;

	/** 精灵平面的高度（UU） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Sprite")
	float PlaneHeight = 100.f;

	// ---- Flipbook 参数 ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Sprite")
	int32 SheetColumns = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Sprite")
	int32 SheetRows = 5; // 5 状态 × 4 方向 或自定义

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Sprite")
	int32 FramesPerAnim = 4;

	/** 重新生成平面 Mesh（尺寸改变后调用） */
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Sprite")
	void RebuildPlaneMesh();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 朝向摄像机（Billboard 行为） */
	void FaceCamera();

	/** 更新 Flipbook UV */
	void UpdateFlipbookUV();

	/** 创建四顶点平面 Mesh Section */
	void CreatePlaneMesh();

	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> DynMaterial;
	UPROPERTY() TObjectPtr<UMaterialInterface> BaseMaterial;

	EWuxiaSpriteDirection CurrentDirection = EWuxiaSpriteDirection::Down;
	EWuxiaSpriteAnimState CurrentAnimState = EWuxiaSpriteAnimState::Idle;

	bool bPlaying = false;
	float CurrentPlayRate = 8.f;
	float FrameTimer = 0.f;
	int32 CurrentFrame = 0;
};
