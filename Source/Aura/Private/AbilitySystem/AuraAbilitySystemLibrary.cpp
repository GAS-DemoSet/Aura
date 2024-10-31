// 


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode || !ASC) return;

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* ClassInfo = AuraGameMode->CharacterClassInfo;
	if (IsValid(ClassInfo))
	{
		FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->FindClassDefaultInfo(CharacterClass);

		/** ！！！ 设置源对象，否则MMC在计算时会出现空指针问题 */
		FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
		PrimaryContextHandle.AddSourceObject(AvatarActor);
		FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);


		FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
		SecondaryContextHandle.AddSourceObject(AvatarActor);
		FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);


		FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
		VitalContextHandle.AddSourceObject(AvatarActor);
		FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, VitalContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
}
