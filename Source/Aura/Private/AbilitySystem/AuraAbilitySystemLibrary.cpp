// 


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlapWidgetController* UAuraAbilitySystemLibrary::GetOverlapWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if (AAuraPlayerState* AuraPS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAuraAbilitySystemComponent* AuraASC = AuraPS->GetAuraAbilitySystemComponent();
				UAuraAttributeSet* AuraAS = AuraPS->GetAuraAttributeSet();
				const FWidgetControllerParam WidgetControllerParam(PC, AuraPS, AuraASC, AuraAS);
				return AuraHUD->GetOverlapWidgetController(WidgetControllerParam);
			}
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if (AAuraPlayerState* AuraPS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAuraAbilitySystemComponent* AuraASC = AuraPS->GetAuraAbilitySystemComponent();
				UAuraAttributeSet* AuraAS = AuraPS->GetAuraAttributeSet();
				const FWidgetControllerParam WidgetControllerParam(PC, AuraPS, AuraASC, AuraAS);
				return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParam);
			}
		}
	}
	return nullptr;
}
