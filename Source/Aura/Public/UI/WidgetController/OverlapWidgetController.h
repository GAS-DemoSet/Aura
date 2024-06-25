#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlapWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangeSignature, float, NewMaxMana);

struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlapWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	//~ Begin UAuraWidgetController interface
	virtual void BroadcastInitializeValue() override;
	virtual void BindCallbackToDependencies() override;
	//~ End UAuraWidgetController interface

private:
	void EventOnHealthChange(const FOnAttributeChangeData& Data) const;
	void EventOnMaxHealthChange(const FOnAttributeChangeData& Data) const;
	void EventOnManaChange(const FOnAttributeChangeData& Data) const;
	void EventOnMaxManaChange(const FOnAttributeChangeData& Data) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangeSignature OnMaxHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnManaChangeSignature OnManaChange;

	UPROPERTY(BlueprintAssignable)
	FOnMaxManaChangeSignature OnMaxManaChange;
};
