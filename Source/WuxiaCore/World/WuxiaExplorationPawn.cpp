// WuxiaExplorationPawn.cpp
#include "World/WuxiaExplorationPawn.h"
#include "World/WuxiaCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AWuxiaExplorationPawn::AWuxiaExplorationPawn()
{
	// 弹簧臂
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->SetRelativeRotation(FRotator(-45.f, -45.f, 0.f));

	// 等轴测摄像机
	IsometricCamera = CreateDefaultSubobject<UWuxiaCameraComponent>(TEXT("IsometricCamera"));
	IsometricCamera->SetupAttachment(SpringArm);
	IsometricCamera->SetRelativeRotation(FRotator::ZeroRotator); // 旋转由弹簧臂控制
}
