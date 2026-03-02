// WuxiaAbilityData.h — 武功数据配置（DataAsset）
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaAbilityData.generated.h"

class UGameplayEffect;
class UTexture2D;

UCLASS(BlueprintType)
class WUXIACORE_API UWuxiaAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ---- 基础信息 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	FText AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "基本信息")
	TSoftObjectPtr<UTexture2D> Icon;

	// ---- 范围与消耗 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "战斗")
	EAbilityRangePattern RangePattern = EAbilityRangePattern::Single;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "战斗")
	int32 Range = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "消耗")
	float InternalEnergyCost = 10.f;

	// ---- 伤害 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "伤害")
	EWuxiaDamageType DamageType = EWuxiaDamageType::External;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "伤害")
	float BasePower = 20.f;

	// ---- 附加效果 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "效果")
	TSubclassOf<UGameplayEffect> AdditionalEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "效果")
	float EffectChance = 0.3f;

	// ---- 学习条件 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "学习")
	FGameplayTagContainer LearnConditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "学习")
	float RequiredAptitude = 0.f;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("WuxiaAbility", GetFName());
	}
};
