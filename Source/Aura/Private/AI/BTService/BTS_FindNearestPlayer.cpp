


#include "AI/BTService/BTS_FindNearestPlayer.h"
#include "AIController.h"

void UBTS_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Black, *AIOwner.GetName());
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, *ActorOwner.GetName());
}
