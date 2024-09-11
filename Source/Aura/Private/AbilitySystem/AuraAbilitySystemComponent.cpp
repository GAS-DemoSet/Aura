#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 此处为服务器调用事件
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* InAbilitySystemComponent,
                                                const FGameplayEffectSpec& InEffectSpec, FActiveGameplayEffectHandle InActiveEffectHandle)
{
	EffectApplied_OnlyClient(InAbilitySystemComponent, InEffectSpec, InActiveEffectHandle);
}

void UAuraAbilitySystemComponent::EffectApplied_OnlyClient_Implementation(
	UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayEffectSpec& InEffectSpec,
	FActiveGameplayEffectHandle InActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	InEffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTagsDelegate.Broadcast(TagContainer);
}
