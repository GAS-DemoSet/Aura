// 


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Log/AuraLog.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// InitHealth(50.f);
	// InitMana(10.f);

	/* Primary Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_BlockChance, GetBlockChanceAttribute);	
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/** Resistance Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get()->Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 这个函数体内执行更改得是 CurrentValue
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange:: Health=%f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange:: Mana=%f"), NewValue);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectPro;
	SetEffectProperties(Data, EffectPro);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("UAuraAttributeSet::PostGameplayEffectExecute:: Health:: %f"), GetHealth()));
		// UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PostGameplayEffectExecute:: Base=%f"), Health.GetBaseValue());
		// UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PostGameplayEffectExecute:: Current=%f"), Health.GetCurrentValue());
		// 需要再次修正 BaseValue
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange:: TargetActor=%s Health=%f"), *EffectPro.TargetAvatarActor->GetName(), GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectPro.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get()->Effect_HitReact);
				EffectPro.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			bool bBlocked = UAuraAbilitySystemLibrary::IsBlockedHit(EffectPro.EffectContextHandle);
			bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(EffectPro.EffectContextHandle);

			ShowFloatingText(EffectPro, LocalIncomingDamage, bBlocked, bCritical);
		}
	}
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldValue);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldValue);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldValue);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldValue);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldValue);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldValue);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldValue);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldValue);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldValue);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldValue);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldValue);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldValue);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldValue);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldValue);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldValue);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectPro) const
{
	// 获取原属性和目标属性
	// 获取源信息方式
	EffectPro.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectPro.SourceASC = EffectPro.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(EffectPro.SourceASC) && EffectPro.SourceASC->AbilityActorInfo.IsValid() && EffectPro.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectPro.SourceAvatarActor = EffectPro.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectPro.SourceController = EffectPro.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (EffectPro.SourceController == nullptr && EffectPro.SourceAvatarActor != nullptr)
		{
			if (APawn* TempPawn = Cast<APawn>(EffectPro.SourceAvatarActor))
			{
				EffectPro.SourceController = TempPawn->GetController();
			}
		}
		if (EffectPro.SourceController != nullptr)
		{
			EffectPro.SourceCharacter = Cast<ACharacter>(EffectPro.SourceController->GetPawn());
		}
	}
	
	// 获取目标信息方式
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectPro.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectPro.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectPro.TargetCharacter = Cast<ACharacter>(EffectPro.TargetAvatarActor);
		EffectPro.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectPro.TargetAvatarActor);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& EffectProp, float Value, bool bBlockHit, bool bCriticalHit)
{
	UE_LOG(AuraLog, Warning, TEXT("UAuraAttributeSet::ShowFloatingText:: %f-- %i"), Value, static_cast<int32>(EffectProp.SourceCharacter->GetLocalRole()));
	if (EffectProp.SourceCharacter && EffectProp.SourceCharacter != EffectProp.TargetCharacter)
	{
		if (AAuraPlayerController* SourceAuraPC = EffectProp.SourceCharacter->GetController<AAuraPlayerController>())
		{
			SourceAuraPC->ShowDamageNumber(Value, EffectProp.TargetCharacter, bBlockHit, bCriticalHit);
		}
		else if (AAuraPlayerController* TargetAuraPC = EffectProp.TargetCharacter->GetController<AAuraPlayerController>())
		{
			TargetAuraPC->ShowDamageNumber(Value, EffectProp.TargetCharacter, bBlockHit, bCriticalHit);
			return;
		}
	}
}

