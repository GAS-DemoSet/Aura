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
#include "Interface/CombatInterface.h"

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

void UAuraAbilitySystemLibrary::GiveStartupAbility(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo && ASC)
	{
		for (const auto& Iter : ClassInfo->CommonAbility)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Iter, 1);
			ASC->GiveAbility(AbilitySpec);
		}

		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			// 在此处注册不同类型角色的不同技能
			const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->FindClassDefaultInfo(CharacterClass);
			for (const auto& Iter : DefaultInfo.StartupAbility)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Iter, CombatInterface->GetPlayerLevel());
				ASC->GiveAbility(AbilitySpec);
			}
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

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutLivePlayer, const TArray<AActor*> IgnoreActor, float Radius, const FVector& SphereOrigin)
{
	// UGameplayStatics::ApplyRadialDamageWithFalloff();
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(IgnoreActor);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const auto& Iter : Overlaps)
		{
			if (Iter.GetActor()->Implements<UCombatInterface>())
			{
				// 活着得玩家
				if (!ICombatInterface::Execute_IsDead(Iter.GetActor()))
				{
					// 添加半径内活着得玩家
					OutLivePlayer.AddUnique(Iter.GetActor());
				}
			}
		}
	}
}
