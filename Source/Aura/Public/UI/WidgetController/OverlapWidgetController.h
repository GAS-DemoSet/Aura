#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlapWidgetController.generated.h"

class UAuraUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangeSignature, float, NewMaxMana);

struct FOnAttributeChangeData;
class UDataTable;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture* Image = nullptr;
};

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

protected:
	template<class T>
	static T* GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag)
	{
		if (InDataTable == nullptr) return nullptr;
		return InDataTable->FindRow<T>(InTag.GetTagName(), TEXT(""));
	}

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
};
