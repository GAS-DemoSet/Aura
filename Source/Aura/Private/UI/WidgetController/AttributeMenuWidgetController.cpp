// 


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitializeValue()
{
	Super::BroadcastInitializeValue();

	UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet.Get());

	// FAuraAttributeInfo StrengthInfo = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get()->Attributes_Primary_Strength);
	// StrengthInfo.AttributeValue = AuraAS->GetStrength();
	// OnAuraAttributeInfoDelegate.Broadcast(StrengthInfo);

	// 广播属性初始值
	for (auto& Pair : AuraAS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	Super::BindCallbackToDependencies();

	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet.Get());

	for (auto& Pair: AuraAS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			this->BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	checkf(AttributeInfo, TEXT("UAttributeMenuWidgetController::BroadcastInitializeValue():: AttributeInfo is nullptr!!!"));
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);

	// 通过执行返回代理获取属性访问器进行取值
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet.Get());
	OnAuraAttributeInfoDelegate.Broadcast(Info);
}
