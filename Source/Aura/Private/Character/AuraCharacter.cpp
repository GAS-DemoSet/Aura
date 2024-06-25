#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 在服务器初始化 ASC 信息
	InitAbilityActorInfo();
}

void AAuraCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 在客户端初始化 ASC 信息
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>();
	check(AuraPS && AuraPS->GetAbilitySystemComponent());
	
	AuraPS->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPS, this);
	AbilitySystemComponent = AuraPS->GetAbilitySystemComponent();
	AttributeSet = AuraPS->GetAttributeSet();

	// 以下操作仅在本地客户端有效
	{
		APlayerController* TempPC = Cast<APlayerController>(GetController());
		if (TempPC)
		{
			AAuraHUD* TempHUD = Cast<AAuraHUD>(TempPC->GetHUD());
			if (TempHUD)
			{
				TempHUD->InitOverlap(TempPC, AuraPS, AbilitySystemComponent, AttributeSet);
			}
		}
	}
}
