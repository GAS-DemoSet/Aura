


#include "AI/BTService/BTS_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTS_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Black, *AIOwner.GetName());
	// GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, *ActorOwner.GetName());

	APawn* OwningPawn = AIOwner->GetPawn();
	if (OwningPawn != nullptr)
	{
		const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

		TArray<AActor*> ActorsWithTag;
		UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);
	}
}
