// 


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// Client
		SendMouseCursorData();
	}
	else
	{
		// TODO:: We are on the server, so listen for target data.
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	if (Ability && Ability->GetCurrentActorInfo())
	{
		if (APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get())
		{
			FHitResult HitRes;
			PC->GetHitResultUnderCursor(ECC_Visibility, false, HitRes);
			// ValidData.Broadcast(HitRes.Location);
			FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(HitRes);

			FGameplayAbilityTargetDataHandle DataHandle;
			DataHandle.Add(Data);

			AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				ValidData.Broadcast(DataHandle);
			}
		}
	}
}
