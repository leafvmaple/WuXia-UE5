// WuxiaGameplayAbility.cpp
#include "Ability/WuxiaGameplayAbility.h"
#include "Ability/WuxiaAbilityData.h"
#include "Character/WuxiaAttributeSet.h"
#include "AbilitySystemComponent.h"

UWuxiaGameplayAbility::UWuxiaGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

EAbilityRangePattern UWuxiaGameplayAbility::GetRangePattern() const
{
	return AbilityData ? AbilityData->RangePattern : EAbilityRangePattern::Single;
}

int32 UWuxiaGameplayAbility::GetRange() const
{
	return AbilityData ? AbilityData->Range : 1;
}

float UWuxiaGameplayAbility::GetInternalEnergyCost() const
{
	return AbilityData ? AbilityData->InternalEnergyCost : 0.f;
}

bool UWuxiaGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	// 检查内力是否足够
	if (AbilityData && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		float CurrentIE = ActorInfo->AbilitySystemComponent->GetNumericAttribute(UWuxiaAttributeSet::GetInternalEnergyAttribute());
		if (CurrentIE < AbilityData->InternalEnergyCost)
			return false;
	}

	return true;
}

void UWuxiaGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 武功逻辑由蓝图或子类实现
	// 基类负责扣除内力（通过 Cost GE）和播放动画

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
