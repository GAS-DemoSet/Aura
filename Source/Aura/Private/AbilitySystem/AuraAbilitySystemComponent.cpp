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
	UE_LOG(LogTemp, Log, TEXT("UAuraAbilitySystemComponent::EffectApplied: %i"), TagContainer.Num());
	for (const FGameplayTag Tag : TagContainer)
	{
		// TODO: Broadcast the tag to the Widget Controller
		const FString Msg = FString::Printf(TEXT("UAuraAbilitySystemComponent::EffectApplied:: GE Tag:%s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Black, Msg);
	}
}
