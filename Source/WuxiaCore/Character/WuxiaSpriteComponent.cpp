// WuxiaSpriteComponent.cpp — ProceduralMesh 版 HD2D Billboard
#include "Character/WuxiaSpriteComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

UWuxiaSpriteComponent::UWuxiaSpriteComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 默认竖直朝向（法线 +Y，面朝摄像机时由 FaceCamera 调整）
	SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	// 渲染设置
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CastShadow = false;
	bUseComplexAsSimpleCollision = false;
}

void UWuxiaSpriteComponent::BeginPlay()
{
	Super::BeginPlay();

	// 在运行时创建平面几何体
	CreatePlaneMesh();

	// 创建动态材质实例
	if (BaseMaterial)
	{
		DynMaterial = CreateDynamicMaterialInstance(0, BaseMaterial);
	}
	else if (GetNumMaterials() > 0 && GetMaterial(0))
	{
		DynMaterial = CreateDynamicMaterialInstance(0, GetMaterial(0));
	}
	else
	{
		// 回退：使用引擎默认 Unlit 材质，确保平面可见
		UMaterialInterface* DefaultMat = LoadObject<UMaterial>(
			nullptr, TEXT("/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial"));
		if (DefaultMat)
		{
			DynMaterial = CreateDynamicMaterialInstance(0, DefaultMat);
			UE_LOG(LogTemp, Warning, TEXT("WuxiaSpriteComponent: 未设置精灵材质，使用引擎默认材质。请创建 M_SpriteBase 并赋值。"));
		}
	}
}

void UWuxiaSpriteComponent::CreatePlaneMesh()
{
	// ---- 4 个顶点构成 1 个平面 ----
	//
	//  3 ---- 2      UV 布局:
	//  |  \   |      0 = (0,1)  左下
	//  |   \  |      1 = (1,1)  右下
	//  0 ---- 1      2 = (1,0)  右上
	//                3 = (0,0)  左上
	//
	// 顶点在 XZ 平面上，法线朝 +Y
	const float HalfW = PlaneWidth * 0.5f;
	const float HalfH = PlaneHeight * 0.5f;

	TArray<FVector> Vertices;
	Vertices.Reserve(4);
	Vertices.Add(FVector(-HalfW, 0.f, -HalfH)); // 0 左下
	Vertices.Add(FVector( HalfW, 0.f, -HalfH)); // 1 右下
	Vertices.Add(FVector( HalfW, 0.f,  HalfH)); // 2 右上
	Vertices.Add(FVector(-HalfW, 0.f,  HalfH)); // 3 左上

	// 两个三角形（逆时针 = 正面朝 +Y）
	TArray<int32> Triangles;
	Triangles.Reserve(6);
	Triangles.Append({ 0, 2, 1 });
	Triangles.Append({ 0, 3, 2 });

	// 法线全部朝 +Y
	TArray<FVector> Normals;
	Normals.Reserve(4);
	Normals.Add(FVector(0.f, 1.f, 0.f));
	Normals.Add(FVector(0.f, 1.f, 0.f));
	Normals.Add(FVector(0.f, 1.f, 0.f));
	Normals.Add(FVector(0.f, 1.f, 0.f));

	// UV（完整 0-1，后续通过材质参数做 sub-UV 偏移）
	TArray<FVector2D> UV0;
	UV0.Reserve(4);
	UV0.Add(FVector2D(0.f, 1.f)); // 左下
	UV0.Add(FVector2D(1.f, 1.f)); // 右下
	UV0.Add(FVector2D(1.f, 0.f)); // 右上
	UV0.Add(FVector2D(0.f, 0.f)); // 左上

	// 切线（沿 +X）
	TArray<FProcMeshTangent> Tangents;
	Tangents.Reserve(4);
	for (int32 i = 0; i < 4; ++i)
	{
		Tangents.Add(FProcMeshTangent(FVector(1.f, 0.f, 0.f), false));
	}

	// 顶点色（白色）
	TArray<FLinearColor> VertexColors;
	VertexColors.Reserve(4);
	for (int32 i = 0; i < 4; ++i)
	{
		VertexColors.Add(FLinearColor::White);
	}

	// 清除旧数据并创建 Section 0
	ClearAllMeshSections();
	CreateMeshSection_LinearColor(
		0,                // SectionIndex
		Vertices,
		Triangles,
		Normals,
		UV0,
		VertexColors,
		Tangents,
		false             // bCreateCollision
	);
}

void UWuxiaSpriteComponent::RebuildPlaneMesh()
{
	CreatePlaneMesh();

	// 重新应用材质
	if (DynMaterial)
	{
		SetMaterial(0, DynMaterial);
	}
}

void UWuxiaSpriteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FaceCamera();

	if (bPlaying)
	{
		FrameTimer += DeltaTime * CurrentPlayRate;
		if (FrameTimer >= 1.f)
		{
			FrameTimer -= 1.f;
			CurrentFrame = (CurrentFrame + 1) % FramesPerAnim;
			UpdateFlipbookUV();
		}
	}
}

void UWuxiaSpriteComponent::FaceCamera()
{
	if (!GetWorld()) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->PlayerCameraManager) return;

	const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	const FVector MyLocation = GetComponentLocation();

	// 只绕 Z 轴旋转朝向摄像机
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyLocation, CameraLocation);
	LookAtRot.Pitch = 90.f; // 保持竖直
	LookAtRot.Roll = 0.f;
	SetWorldRotation(LookAtRot);
}

void UWuxiaSpriteComponent::SetSpriteTexture(UTexture2D* InTexture)
{
	if (DynMaterial && InTexture)
	{
		DynMaterial->SetTextureParameterValue(FName("SpriteTexture"), InTexture);
	}
}

void UWuxiaSpriteComponent::SetSpriteMaterial(UMaterialInterface* InMaterial)
{
	BaseMaterial = InMaterial;
	if (InMaterial)
	{
		DynMaterial = CreateDynamicMaterialInstance(0, InMaterial);
		UpdateFlipbookUV(); // 刷新 UV 参数到新材质
	}
}

void UWuxiaSpriteComponent::SetDirection(EWuxiaSpriteDirection NewDirection)
{
	if (CurrentDirection != NewDirection)
	{
		CurrentDirection = NewDirection;
		CurrentFrame = 0;
		UpdateFlipbookUV();
	}
}

void UWuxiaSpriteComponent::SetAnimState(EWuxiaSpriteAnimState NewState)
{
	if (CurrentAnimState != NewState)
	{
		CurrentAnimState = NewState;
		CurrentFrame = 0;
		FrameTimer = 0.f;

		if (NewState == EWuxiaSpriteAnimState::Idle)
		{
			StopFlipbook();
		}
		else
		{
			PlayFlipbook();
		}

		UpdateFlipbookUV();
	}
}

void UWuxiaSpriteComponent::PlayFlipbook(float InPlayRate)
{
	bPlaying = true;
	CurrentPlayRate = InPlayRate;
	FrameTimer = 0.f;
	CurrentFrame = 0;
}

void UWuxiaSpriteComponent::StopFlipbook()
{
	bPlaying = false;
	CurrentFrame = 0;
	FrameTimer = 0.f;
	UpdateFlipbookUV();
}

void UWuxiaSpriteComponent::UpdateFlipbookUV()
{
	if (!DynMaterial) return;

	// 图集布局：列 = 帧 (SheetColumns), 行 = 方向×状态
	// Row = AnimState * 4 + Direction
	const int32 Row = static_cast<int32>(CurrentAnimState) * 4 + static_cast<int32>(CurrentDirection);
	const int32 Col = CurrentFrame;

	const float U = static_cast<float>(Col) / static_cast<float>(SheetColumns);
	const float V = static_cast<float>(Row) / static_cast<float>(SheetRows);

	DynMaterial->SetScalarParameterValue(FName("UOffset"), U);
	DynMaterial->SetScalarParameterValue(FName("VOffset"), V);
	DynMaterial->SetScalarParameterValue(FName("UScale"), 1.f / SheetColumns);
	DynMaterial->SetScalarParameterValue(FName("VScale"), 1.f / SheetRows);
}
}
