// WuxiaCharacterBase.h — 角色基类，HD2D 精灵角色
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaCharacterBase.generated.h"

class UAbilitySystemComponent;
class UWuxiaAttributeSet;
class UWuxiaCharacterData;
class UWuxiaSpriteComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WUXIACORE_API AWuxiaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWuxiaCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	// ---- 初始化 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Character")
	void InitializeFromData(UWuxiaCharacterData* InData);

	UFUNCTION(BlueprintPure, Category = "Wuxia|Character")
	UWuxiaCharacterData* GetCharacterData() const { return CharacterData; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Character")
	UWuxiaAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintPure, Category = "Wuxia|Character")
	UWuxiaSpriteComponent* GetSpriteComponent() const { return SpriteComp; }

	// ---- 阵营 ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wuxia|Character")
	EWuxiaFaction Faction = EWuxiaFaction::Player;

	// ---- 战斗网格位置 ----
	UPROPERTY(BlueprintReadWrite, Category = "Wuxia|Battle")
	FIntPoint GridPosition = FIntPoint::ZeroValue;

	// ---- 是否存活 ----
	UFUNCTION(BlueprintPure, Category = "Wuxia|Character")
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|GAS")
	TObjectPtr<UWuxiaAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wuxia|Sprite")
	TObjectPtr<UWuxiaSpriteComponent> SpriteComp;

	UPROPERTY(Transient)
	TObjectPtr<UWuxiaCharacterData> CharacterData;

	/** 根据移动方向更新精灵朝向 */
	void UpdateSpriteDirection();
};
