// 

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interface/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	static bool GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages, FTaggedMontage& OutMontage);

protected:
	/** 伤害GE */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 伤害配置表（不同等级伤害值均不同） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
