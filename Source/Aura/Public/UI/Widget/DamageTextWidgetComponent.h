

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageText")
	void SetDamageText(float Value, bool bBlockHit, bool bCriticalHit);
};
