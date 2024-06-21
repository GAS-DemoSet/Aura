#include "Character/AuraCharacterBase.h"

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
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
