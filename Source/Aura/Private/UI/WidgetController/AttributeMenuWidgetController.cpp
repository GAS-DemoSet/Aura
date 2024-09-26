// 


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitializeValue()
{
	Super::BroadcastInitializeValue();

	UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("UAttributeMenuWidgetController::BroadcastInitializeValue():: AttributeInfo is nullptr!!!"));
	// FAuraAttributeInfo StrengthInfo = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get()->Attributes_Primary_Strength);
	// StrengthInfo.AttributeValue = AuraAS->GetStrength();
	// OnAuraAttributeInfoDelegate.Broadcast(StrengthInfo);

	// 广播属性初始值
	for (auto& Pair : AuraAS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);

		// 通过执行返回代理获取属性访问器进行取值
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAS);
		OnAuraAttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	Super::BindCallbackToDependencies();
}
