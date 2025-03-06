#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	for (const auto& Iter : DamageTypes)
	{
		const float ScaledDamage = Iter.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Iter.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

bool UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages,
	FTaggedMontage& OutMontage)
{
	if (TaggedMontages.Num())
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		if (TaggedMontages.IsValidIndex(Selection))
		{
			OutMontage = TaggedMontages[Selection];
			return true;
		}
	}
	return false;
}
