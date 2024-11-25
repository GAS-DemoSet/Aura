

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
public:
	//~ Begin UAbilitySystemGlobals Interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~ End UAbilitySystemGlobals Interface
	
};
