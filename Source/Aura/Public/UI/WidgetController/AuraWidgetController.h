

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParam
{
	GENERATED_BODY()

	FWidgetControllerParam() {}
	FWidgetControllerParam(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC)
		, PlayerState(PS)
		, AbilitySystemComponent(ASC)
		, AttributeSet(AS)
	{}
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParam(const FWidgetControllerParam& WCP);

	/** 广播属性初始值 */
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	virtual void BroadcastInitializeValue() {}

	/** 绑定相关属性的状态变化 */
	virtual void BindCallbackToDependencies() {}

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<UAttributeSet> AttributeSet;
};
