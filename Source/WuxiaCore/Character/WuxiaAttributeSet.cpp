// WuxiaAttributeSet.cpp
#include "Character/WuxiaAttributeSet.h"
#include "GameplayEffectExtension.h"

UWuxiaAttributeSet::UWuxiaAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitInternalEnergy(50.f);
	InitMaxInternalEnergy(50.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
	InitAttack(10.f);
	InitDefense(5.f);
	InitAgility(5.f);
	InitAptitude(10.f);
	InitMoveRange(4.f);
	InitIncomingDamage(0.f);
}

void UWuxiaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp 生命与内力到 [0, Max]
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetInternalEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxInternalEnergy());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UWuxiaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 处理 IncomingDamage Meta 属性
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float DamageDone = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (DamageDone > 0.f)
		{
			const float NewHealth = GetHealth() - DamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		}
	}
}
