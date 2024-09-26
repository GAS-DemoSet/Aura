// 

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
class UAttributeInfo;
struct FAuraAttributeInfo;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuraAttributeInfoSignature, const FAuraAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitializeValue() override;
	virtual void BindCallbackToDependencies() override;

public:
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAuraAttributeInfoSignature OnAuraAttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Widget Data")
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
