// WuxiaNPC.h — NPC Actor（精灵 + 交互触发器）
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WuxiaNPC.generated.h"

class UWuxiaSpriteComponent;
class UWuxiaDialogueData;
class USphereComponent;

UCLASS()
class WUXIACORE_API AWuxiaNPC : public AActor
{
	GENERATED_BODY()

public:
	AWuxiaNPC();

	// ---- 交互 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|NPC")
	void Interact(AActor* Interactor);

	UFUNCTION(BlueprintPure, Category = "Wuxia|NPC")
	bool IsPlayerInRange() const { return bPlayerInRange; }

	// ---- 数据 ----
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|NPC")
	FText NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|NPC")
	TObjectPtr<UWuxiaDialogueData> DialogueData;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|Sprite")
	TObjectPtr<UWuxiaSpriteComponent> SpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|NPC")
	TObjectPtr<USphereComponent> InteractionTrigger;

	bool bPlayerInRange = false;
};
