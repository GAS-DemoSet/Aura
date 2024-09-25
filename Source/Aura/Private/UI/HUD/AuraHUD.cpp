#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlapWidgetController.h"

UAuraUserWidget* AAuraHUD::GetOverlapWidget() const
{
	return OverlapWidget;
}

UOverlapWidgetController* AAuraHUD::GetOverlapWidgetController(const FWidgetControllerParam& WCP)
{
	if (OverlapWidgetController == nullptr)
	{
		OverlapWidgetController = NewObject<UOverlapWidgetController>(this, OverlapWidgetControllerClass);
		OverlapWidgetController->SetWidgetControllerParam(WCP);
		OverlapWidgetController->BindCallbackToDependencies();
	}
	return OverlapWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParam& WCP)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParam(WCP);
		AttributeMenuWidgetController->BindCallbackToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlapWidgetClass, TEXT("OverlapWidgetClass uninitialize, param fill out BP_AuraHUD!!!"));
	checkf(OverlapWidgetControllerClass, TEXT("OverlapWidgetControllerClass uninitialize, param fill out BP_AuraHUD!!!"));
	
	OverlapWidget = CreateWidget<UAuraUserWidget>(this->GetWorld(), OverlapWidgetClass);

	const FWidgetControllerParam WCP(PC, PS, ASC, AS);
	UOverlapWidgetController* TempWC = GetOverlapWidgetController(WCP);
	OverlapWidget->SetWidgetController(TempWC);
	// 相关变量和绑定初始化完成后，广播初始参数值
	TempWC->BroadcastInitializeValue();
	
	OverlapWidget->AddToViewport();
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	
}
