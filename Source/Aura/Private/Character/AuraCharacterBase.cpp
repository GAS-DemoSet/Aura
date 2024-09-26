#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Log/AuraLog.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeaponMesh() const
{
	return Weapon;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

UAuraAbilitySystemComponent* AAuraCharacterBase::GetAuraAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* AAuraCharacterBase::GetAuraAttributeSet() const
{
	return AuraAttributeSet;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GE, float Level) const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (GE)
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE, Level, ContextHandle);
			ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
		}
		else
		{
			UE_LOG(AuraLog, Warning, TEXT("AAuraCharacterBase::ApplyEffectToSelf:: GE is nullptr!!!"));
		}
	}
	else
	{
		UE_LOG(AuraLog, Warning, TEXT("AAuraCharacterBase::ApplyEffectToSelf:: ASC is nullptr!!!"));
	}
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	// 
	if (!HasAuthority()) return;

	checkf(AuraAbilitySystemComponent, TEXT("Error: AAuraCharacterBase::AddCharacterAbilities:: The ASC component is not initialized!!!"));
	AuraAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
