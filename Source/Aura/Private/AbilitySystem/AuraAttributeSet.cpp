// 


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(10.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 这个函数体内执行更改得是 CurrentValue
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		// UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange:: Health=%f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		// UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange:: Mana=%f"), NewValue);
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
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
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

