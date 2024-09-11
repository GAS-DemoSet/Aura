// 


#include "UI/WidgetController/OverlapWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlapWidgetController::BroadcastInitializeValue()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChange.Broadcast(AuraAS->GetHealth());
	OnMaxHealthChange.Broadcast(AuraAS->GetMaxHealth());
	OnManaChange.Broadcast(AuraAS->GetMana());
	OnMaxManaChange.Broadcast(AuraAS->GetMaxMana());
}

void UOverlapWidgetController::BindCallbackToDependencies()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

	// 绑定属性值状态变化
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnMaxHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetManaAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnManaChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxManaAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnMaxManaChange);

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->EffectAssetTagsDelegate.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag Tag : AssetTags)
		{
			// TODO: Broadcast the tag to the Widget Controller
			const FString Msg = FString::Printf(TEXT("EffectApplied:: GE Tag:%s"), *Tag.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Black, Msg);

			FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
		}
	});
}

void UOverlapWidgetController::EventOnHealthChange(const FOnAttributeChangeData& Data) const
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void UOverlapWidgetController::EventOnMaxHealthChange(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChange.Broadcast(Data.NewValue);
}

void UOverlapWidgetController::EventOnManaChange(const FOnAttributeChangeData& Data) const
{
	OnManaChange.Broadcast(Data.NewValue);
}

void UOverlapWidgetController::EventOnMaxManaChange(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChange.Broadcast(Data.NewValue);
}
