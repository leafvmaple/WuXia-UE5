// WuxiaNPC.cpp
#include "NPC/WuxiaNPC.h"
#include "Character/WuxiaSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AWuxiaNPC::AWuxiaNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	SpriteComp = CreateDefaultSubobject<UWuxiaSpriteComponent>(TEXT("Sprite"));
	SpriteComp->SetupAttachment(RootComponent);
	SpriteComp->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));
	InteractionTrigger->SetupAttachment(RootComponent);
	InteractionTrigger->SetSphereRadius(150.f);
	InteractionTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractionTrigger->SetGenerateOverlapEvents(true);
}

void AWuxiaNPC::BeginPlay()
{
	Super::BeginPlay();

	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWuxiaNPC::OnOverlapBegin);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &AWuxiaNPC::OnOverlapEnd);
}

void AWuxiaNPC::Interact(AActor* Interactor)
{
	if (!DialogueData) return;

	// 触发对话系统（通过 GameMode 或 PlayerController 转发）
	// 具体实现在 DialogueComponent 中
	UE_LOG(LogTemp, Log, TEXT("NPC [%s] interacted by [%s]"), *NPCName.ToString(), *Interactor->GetName());
}

void AWuxiaNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA<ACharacter>())
	{
		bPlayerInRange = true;
	}
}

void AWuxiaNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA<ACharacter>())
	{
		bPlayerInRange = false;
	}
}
