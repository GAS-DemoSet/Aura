


#include "AI/BTService/BTS_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Log/AuraLog.h"

void UBTS_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Black, *AIOwner.GetName());
	// GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, *ActorOwner.GetName());

	APawn* OwningPawn = AIOwner->GetPawn();
	if (OwningPawn != nullptr)
	{
		const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
		
		Print_Log_NetRole(OwningPawn, Warning, *TargetTag.ToString());

		TArray<AActor*> ActorsWithTag;
		UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

		float ClosestDistance = TNumericLimits<float>::Max();
		AActor* ClosestActor = nullptr;

		for (AActor* Actor : ActorsWithTag)
		{
			if (Actor)
			{
				const float Distance = OwningPawn->GetDistanceTo(Actor);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestActor = Actor;
				}
			}
		}

		// 设置黑板值
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
	}
}
