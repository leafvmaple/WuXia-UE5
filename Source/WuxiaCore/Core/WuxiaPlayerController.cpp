// WuxiaPlayerController.cpp
#include "Core/WuxiaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputModifiers.h"

AWuxiaPlayerController::AWuxiaPlayerController()
{
	bShowMouseCursor = false;
}

void AWuxiaPlayerController::CreateDefaultInputAssets()
{
	// ---- 如果编辑器里没有配置输入资产，生成运行时默认版本 ----
	if (!MoveAction)
	{
		MoveAction = NewObject<UInputAction>(this, TEXT("IA_Move_Default"));
		MoveAction->ValueType = EInputActionValueType::Axis2D;
	}

	if (!InteractAction)
	{
		InteractAction = NewObject<UInputAction>(this, TEXT("IA_Interact_Default"));
		InteractAction->ValueType = EInputActionValueType::Boolean;
	}

	if (!ExplorationMappingContext)
	{
		ExplorationMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Exploration_Default"));

		// WASD 移动
		{
			FEnhancedActionKeyMapping& WMapping = ExplorationMappingContext->MapKey(MoveAction, EKeys::W);
			UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>();
			Swizzle->Order = EInputAxisSwizzle::YXZ;
			WMapping.Modifiers.Add(Swizzle);
		}
		{
			FEnhancedActionKeyMapping& SMapping = ExplorationMappingContext->MapKey(MoveAction, EKeys::S);
			UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>();
			Swizzle->Order = EInputAxisSwizzle::YXZ;
			SMapping.Modifiers.Add(Swizzle);
			UInputModifierNegate* Negate = NewObject<UInputModifierNegate>();
			SMapping.Modifiers.Add(Negate);
		}
		{
			ExplorationMappingContext->MapKey(MoveAction, EKeys::D);
		}
		{
			FEnhancedActionKeyMapping& AMapping = ExplorationMappingContext->MapKey(MoveAction, EKeys::A);
			UInputModifierNegate* Negate = NewObject<UInputModifierNegate>();
			AMapping.Modifiers.Add(Negate);
		}

		// E 键交互
		ExplorationMappingContext->MapKey(InteractAction, EKeys::E);
	}
}

void AWuxiaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 如果没有在编辑器中配置输入资产，创建运行时默认版
	CreateDefaultInputAssets();

	// 添加探索输入映射
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (ExplorationMappingContext)
		{
			Subsystem->AddMappingContext(ExplorationMappingContext, 0);
		}
	}

	SetupIsometricCamera();
}

void AWuxiaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 确保输入资产存在
	CreateDefaultInputAssets();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWuxiaPlayerController::HandleMoveInput);
		}
		if (InteractAction)
		{
			EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AWuxiaPlayerController::HandleInteractInput);
		}
	}
}

void AWuxiaPlayerController::SwitchInputForPhase(EWuxiaGamePhase Phase)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem) return;

	// 清除所有映射，然后按阶段添加
	Subsystem->ClearAllMappings();

	switch (Phase)
	{
	case EWuxiaGamePhase::Exploration:
		if (ExplorationMappingContext)
			Subsystem->AddMappingContext(ExplorationMappingContext, 0);
		SetInputMode(FInputModeGameOnly());
		break;

	case EWuxiaGamePhase::Battle:
		if (BattleMappingContext)
			Subsystem->AddMappingContext(BattleMappingContext, 0);
		SetInputMode(FInputModeGameAndUI());
		break;

	case EWuxiaGamePhase::Dialogue:
	case EWuxiaGamePhase::Menu:
		SetInputMode(FInputModeUIOnly());
		break;
	}
}

void AWuxiaPlayerController::SetupIsometricCamera()
{
	// 等轴测摄像机设置将在 Pawn 的 SpringArm + Camera 上配置
	// 此处可做额外的摄像机逻辑初始化
}

void AWuxiaPlayerController::HandleMoveInput(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// 等轴测方向：将输入映射到世界 XY
		const FRotator YawRotation(0.f, 45.f, 0.f); // 45度修正
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDir, MoveValue.Y);
		ControlledPawn->AddMovementInput(RightDir, MoveValue.X);
	}
}

void AWuxiaPlayerController::HandleInteractInput()
{
	// 交互逻辑：检测面前的可交互 Actor（NPC、宝箱等）
	// 由子类或蓝图实现具体交互
}
