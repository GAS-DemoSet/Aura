


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AbilitySystem/AuraAbilityType.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
