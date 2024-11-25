// 


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityType.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

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
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo && ASC)
	{
		AActor* AvatarActor = ASC->GetAvatarActor();
		
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

void UAuraAbilitySystemLibrary::GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo && ASC)
	{
		for (const auto& Iter : ClassInfo->CommonAbility)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Iter, 1);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return AuraGameMode != nullptr ? AuraGameMode->CharacterClassInfo : nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraGameplayEffectContext != nullptr)
	{
		return  AuraGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraGameplayEffectContext != nullptr)
	{
		return  AuraGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bNewBlockedHit)
{
	FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraGameplayEffectContext != nullptr)
	{
		AuraGameplayEffectContext->SetIsBlockedHit(bNewBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bNewCriticalHit)
{
	FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraGameplayEffectContext != nullptr)
	{
		AuraGameplayEffectContext->SetIsCriticalHit(bNewCriticalHit);
	}
}
