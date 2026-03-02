// WuxiaCharacterBase.cpp
#include "Character/WuxiaCharacterBase.h"
#include "Character/WuxiaAttributeSet.h"
#include "Character/WuxiaCharacterData.h"
#include "Character/WuxiaSpriteComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AWuxiaCharacterBase::AWuxiaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// GAS
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UWuxiaAttributeSet>(TEXT("AttributeSet"));

	// 精灵组件
	SpriteComp = CreateDefaultSubobject<UWuxiaSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);
	SpriteComp->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	// 隐藏默认骨骼网格体（使用精灵代替）
	GetMesh()->SetVisibility(false);

	// 移动设置
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = 300.f;
		MoveComp->bOrientRotationToMovement = false;
	}

	bUseControllerRotationYaw = false;
}

void AWuxiaCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 初始化 GAS
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AWuxiaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSpriteDirection();
}

void AWuxiaCharacterBase::InitializeFromData(UWuxiaCharacterData* InData)
{
	if (!InData) return;
	CharacterData = InData;

	// 从 DataAsset 初始化属性
	if (AttributeSet)
	{
		AttributeSet->InitHealth(InData->BaseHealth);
		AttributeSet->InitMaxHealth(InData->BaseHealth);
		AttributeSet->InitInternalEnergy(InData->BaseInternalEnergy);
		AttributeSet->InitMaxInternalEnergy(InData->BaseInternalEnergy);
		AttributeSet->InitStamina(InData->BaseStamina);
		AttributeSet->InitMaxStamina(InData->BaseStamina);
		AttributeSet->InitAttack(InData->BaseAttack);
		AttributeSet->InitDefense(InData->BaseDefense);
		AttributeSet->InitAgility(InData->BaseAgility);
		AttributeSet->InitAptitude(InData->BaseAptitude);
		AttributeSet->InitMoveRange(InData->BaseMoveRange);
	}

	// 设置精灵图集
	if (SpriteComp && InData->SpriteSheet.IsValid())
	{
		SpriteComp->SetSpriteTexture(InData->SpriteSheet.LoadSynchronous());
	}
}

bool AWuxiaCharacterBase::IsAlive() const
{
	return AttributeSet && AttributeSet->GetHealth() > 0.f;
}

void AWuxiaCharacterBase::UpdateSpriteDirection()
{
	if (!SpriteComp) return;

	const FVector Velocity = GetVelocity();
	if (Velocity.SizeSquared2D() < 1.f) return;

	// 判断主方向
	const float AbsX = FMath::Abs(Velocity.X);
	const float AbsY = FMath::Abs(Velocity.Y);

	EWuxiaSpriteDirection NewDir;
	if (AbsX > AbsY)
	{
		NewDir = Velocity.X > 0.f ? EWuxiaSpriteDirection::Right : EWuxiaSpriteDirection::Left;
	}
	else
	{
		NewDir = Velocity.Y > 0.f ? EWuxiaSpriteDirection::Down : EWuxiaSpriteDirection::Up;
	}

	SpriteComp->SetDirection(NewDir);
}
