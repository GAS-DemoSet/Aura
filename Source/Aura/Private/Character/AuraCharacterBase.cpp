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

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
