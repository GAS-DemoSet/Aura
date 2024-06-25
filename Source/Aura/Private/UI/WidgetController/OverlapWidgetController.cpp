// 


#include "UI/WidgetController/OverlapWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlapWidgetController::BroadcastInitializeValue()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChange.Broadcast(AuraAS->GetHealth());
	OnMaxHealthChange.Broadcast(AuraAS->GetMaxHealth());
}
