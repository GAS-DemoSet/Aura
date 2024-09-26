#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 此处为服务器调用事件
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto &Iter: StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Iter);
		if (const UAuraGameplayAbility* AuraGA = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGA->StartupInputTag);
			// 注册能力
			GiveAbility(AbilitySpec);
		}

		// 注册能力并且激活一次执行
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& Iter : GetActivatableAbilities())
	{
		if (Iter.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(Iter);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& Iter : GetActivatableAbilities())
	{
		if (Iter.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Iter);
			if (!Iter.IsActive())
			{
				TryActivateAbility(Iter.Handle);
			}
		}
	}
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
