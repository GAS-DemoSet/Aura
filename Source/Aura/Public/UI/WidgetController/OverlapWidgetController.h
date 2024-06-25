#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlapWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeSignature, float, NewMaxHealth);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlapWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitializeValue() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangeSignature OnMaxHealthChange;
};
