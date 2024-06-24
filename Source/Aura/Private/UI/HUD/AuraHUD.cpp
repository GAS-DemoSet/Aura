


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"

UAuraUserWidget* AAuraHUD::GetOverlapWidget() const
{
	return OverlapWidget;
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	OverlapWidget = CreateWidget<UAuraUserWidget>(this->GetWorld(), OverlapWidgetClass);
	OverlapWidget->AddToViewport();
}
