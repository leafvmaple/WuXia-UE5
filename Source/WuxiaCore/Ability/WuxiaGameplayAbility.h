// WuxiaGameplayAbility.h — 武功基类（GAS 能力）
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaGameplayAbility.generated.h"

class UWuxiaAbilityData;

UCLASS()
class WUXIACORE_API UWuxiaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UWuxiaGameplayAbility();

	// ---- 武功数据 ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Ability")
	TObjectPtr<UWuxiaAbilityData> AbilityData;

	// ---- 攻击范围查询 ----
	UFUNCTION(BlueprintPure, Category = "Wuxia|Ability")
	EAbilityRangePattern GetRangePattern() const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Ability")
	int32 GetRange() const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Ability")
	float GetInternalEnergyCost() const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
