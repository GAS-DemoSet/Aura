

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UOverlapWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParam;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	/** 获取主 UI 控件 */
	UAuraUserWidget* GetOverlapWidget() const;

	UOverlapWidgetController* GetOverlapWidgetController(const FWidgetControllerParam& WCP);

	UFUNCTION(BlueprintCallable)
	void InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:
	virtual void BeginPlay() override;

protected:
	/** 主 UI 控件 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAuraUserWidget> OverlapWidget;

	/** 主 UI 控制器 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UOverlapWidgetController> OverlapWidgetController;

private:
	/** 生成的主 UI 控件类型 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAuraUserWidget> OverlapWidgetClass;

	/** 生成的主 UI 控件器类型 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverlapWidgetController> OverlapWidgetControllerClass;
};
