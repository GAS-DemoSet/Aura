#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* InAbilitySystemComponent,
                                                const FGameplayEffectSpec& InEffectSpec, FActiveGameplayEffectHandle InActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	InEffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
