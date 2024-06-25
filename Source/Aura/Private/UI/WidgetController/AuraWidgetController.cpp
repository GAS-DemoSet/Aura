#include "UI/WidgetController/AuraWidgetController.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerController.h"

void UAuraWidgetController::SetWidgetControllerParam(const FWidgetControllerParam& WCP)
{
	PlayerController = WCP.PlayerController;
	PlayerState = WCP.PlayerState;
	AbilitySystemComponent = WCP.AbilitySystemComponent;
	AttributeSet = WCP.AttributeSet;
}

void UAuraWidgetController::BroadcastInitializeValue()
{
}

void UAuraWidgetController::BindCallbackToDependencies()
{
}
