#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
		// Client 本地客户端收集数据并发送服务器
		SendMouseCursorData();
	}
	else
	{
		// TODO:: We are on the server, so listen for target data.
		// 服务器等待客户端正确数据
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//AbilitySystemComponent.Get()->AbilityTargetDataCancelledDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCancel);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			// 等待远端数据（回调UTargetDataUnderMouse::OnTargetDataReplicatedCallback）
			SetWaitingOnRemotePlayerData();
		}
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

			// 向服务器发送 TargetData
			AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);
			//AbilitySystemComponent->ServerSetReplicatedTargetDataCancelled(GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
			
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				ValidData.Broadcast(DataHandle);
			}
		}
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag GameplayTag)
{
	// 这个函数仅在服务器执行
	// 告诉客户端 服务器已经收到了数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	//FString PrintStr = FString::Printf(TEXT("UTargetDataUnderMouse::OnTargetDataReplicatedCallback: %s"), Ability->GetAvatarActorFromActorInfo()->HasAuthority() ? TEXT("服务器！！！") : TEXT("客户端"));
	//UKismetSystemLibrary::PrintString(AbilitySystemComponent.Get(), PrintStr, true, true, FLinearColor::Black, 10.f);
	
	// 在服务器广播正确数据
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCancel()
{
	// 告诉客户端 服务器已经收到了数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	FString PrintStr = FString::Printf(TEXT("UTargetDataUnderMouse::OnTargetDataReplicatedCancel"));
	UKismetSystemLibrary::PrintString(AbilitySystemComponent.Get(), PrintStr, true, true, FLinearColor::Black, 10.f);
}
