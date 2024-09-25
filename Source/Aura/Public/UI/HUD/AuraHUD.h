

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
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
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParam& WCP);

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

	/** 属性菜单控制器 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

private:
	/** 生成的主 UI 控件类型 */
	UPROPERTY(EditDefaultsOnly, Category="AuraHUD|SpawnClass")
	TSubclassOf<UAuraUserWidget> OverlapWidgetClass;

	/** 生成的主 UI 控件器类型 */
	UPROPERTY(EditDefaultsOnly, Category="AuraHUD|SpawnClass")
	TSubclassOf<UOverlapWidgetController> OverlapWidgetControllerClass;

	/** 生成的属性菜单控制器类型 */
	UPROPERTY(EditDefaultsOnly, Category="AuraHUD|SpawnClass")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
