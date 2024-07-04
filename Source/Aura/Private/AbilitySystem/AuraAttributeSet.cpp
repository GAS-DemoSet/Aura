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
	InitMana(50.f);
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
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		//UE_LOG(LogTemp, Warning, TEXT("Health=%f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
		//UE_LOG(LogTemp, Warning, TEXT("Mana=%f"), NewValue);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/*
	// if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Health From GetHealth()=%f"), GetHealth());
	// 	// 当前效果作用值
	// 	UE_LOG(LogTemp, Warning, TEXT("Magnitude=%f"), Data.EvaluatedData.Magnitude);
	// }

	// // 获取源信息方式
	// const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	// const UAbilitySystemComponent *SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	// if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	// {
	// 	AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
	// 	const AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
	// 	if (SourceController == nullptr && SourceAvatarActor != nullptr)
	// 	{
	// 		if (APawn* TempPawn = Cast<APawn>(SourceAvatarActor))
	// 		{
	// 			SourceController = TempPawn->GetController();
	// 		}
	// 	}
	// 	if (SourceController != nullptr)
	// 	{
	// 		ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
	// 	}
	// }
	//
	// // 获取目标信息方式
	// if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	// {
	// 	AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	// 	AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
	// 	ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
	// 	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
	// }
	*/

	FEffectProperties EffectPro;
	SetEffectProperties(Data, EffectPro);
	
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

