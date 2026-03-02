// WuxiaCharacterData.h — 数据驱动角色配置（DataAsset）
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WuxiaCharacterData.generated.h"

class UWuxiaAbilityData;
class UTexture2D;

UCLASS(BlueprintType)
class WUXIACORE_API UWuxiaCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ---- 基础信息 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	FText CharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	TSoftObjectPtr<UTexture2D> Portrait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	TSoftObjectPtr<UTexture2D> SpriteSheet;

	// ---- 基础属性 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseInternalEnergy = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseAttack = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseDefense = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseAgility = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseAptitude = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "属性")
	float BaseMoveRange = 4.f;

	// ---- 可学武功 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "武功")
	TArray<TSoftObjectPtr<UWuxiaAbilityData>> LearnableAbilities;

	// ---- 加入条件（GameplayTag） ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "招募")
	FGameplayTagContainer JoinConditions;

	// ---- DataAsset ID ----
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("WuxiaCharacter", GetFName());
	}
};
