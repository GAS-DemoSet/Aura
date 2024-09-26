// 

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;

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
};
