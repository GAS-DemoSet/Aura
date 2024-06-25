// 


#include "UI/WidgetController/OverlapWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlapWidgetController::BroadcastInitializeValue()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChange.Broadcast(AuraAS->GetHealth());
	OnMaxHealthChange.Broadcast(AuraAS->GetMaxHealth());
}

void UOverlapWidgetController::BindCallbackToDependencies()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

	// 绑定属性值状态变化
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlapWidgetController::EventOnMaxHealthChange);
}

void UOverlapWidgetController::EventOnHealthChange(const FOnAttributeChangeData& Data) const
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void UOverlapWidgetController::EventOnMaxHealthChange(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChange.Broadcast(Data.NewValue);
}
