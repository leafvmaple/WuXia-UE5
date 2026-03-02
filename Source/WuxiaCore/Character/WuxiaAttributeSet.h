// WuxiaAttributeSet.h — GAS 属性集，武侠核心属性
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WuxiaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class WUXIACORE_API UWuxiaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWuxiaAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ---- 生命 ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, MaxHealth)

	// ---- 内力 ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData InternalEnergy;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, InternalEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData MaxInternalEnergy;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, MaxInternalEnergy)

	// ---- 体力 ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, MaxStamina)

	// ---- 攻击力 ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Attack)

	// ---- 防御力 ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Defense)

	// ---- 轻功（速度/闪避） ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Agility)

	// ---- 根骨（成长资质） ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData Aptitude;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, Aptitude)

	// ---- 移动力（战斗网格步数） ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData MoveRange;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, MoveRange)

	// ---- 临时：受到的伤害（Meta Attribute，不持久化） ----
	UPROPERTY(BlueprintReadOnly, Category = "Wuxia|Attributes") FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UWuxiaAttributeSet, IncomingDamage)
};
