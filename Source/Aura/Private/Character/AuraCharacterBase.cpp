#include "Character/AuraCharacterBase.h"

#include "Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Log/AuraLog.h"
#include "AuraGameplayTags.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	

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

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	EventHandleDeath_Multicast();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get()->CombatSocket_Weapon) && !WeaponTipSocketName.IsNone())
	{
		check(Weapon != nullptr)
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	else if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get()->CombatSocket_LeftHand) && !LeftHandSocketName.IsNone())
	{
		check(GetMesh() != nullptr)
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	else if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get()->CombatSocket_RightHand) && !RightHandSocketName.IsNone())
	{
		check(GetMesh() != nullptr)
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	else if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get()->CombatSocket_Tail) && !TailSocketName.IsNone())
	{
		check(GetMesh() != nullptr)
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector::ZeroVector;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	// return ICombatInterface::IsDead_Implementation();
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	// return ICombatInterface::GetAvatar_Implementation();
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
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

void AAuraCharacterBase::Dissolve()
{
	UMaterialInstanceDynamic* DynamicMatIns = nullptr;
	UMaterialInstanceDynamic* WeaponDynamicMatIns = nullptr;
	if (DissolveMaterialInstance)
	{
		DynamicMatIns = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatIns);
	}
	if (WeaponDissolveMaterialInstance)
	{
		WeaponDynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatIns);
	}
	
	StartDissolveTimeline(DynamicMatIns, WeaponDynamicMatIns);
}

void AAuraCharacterBase::EventHandleDeath_Multicast_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	Dissolve();

	bDead = false;
}
