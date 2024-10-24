// 


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	if (Ability && Ability->GetCurrentActorInfo())
	{
		if (APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get())
		{
			FHitResult HitRes;
			PC->GetHitResultUnderCursor(ECC_Visibility, false, HitRes);
			ValidData.Broadcast(HitRes.Location);
		}
	}
}
