

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UAuraUserWidget* GetOverlapWidget() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAuraUserWidget> OverlapWidget;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAuraUserWidget> OverlapWidgetClass;
};
