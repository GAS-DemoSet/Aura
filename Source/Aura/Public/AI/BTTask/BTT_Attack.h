

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTT_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()


public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// virtual FString GetStaticDescription() const override;
	// virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	// virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;
	// virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	// virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
