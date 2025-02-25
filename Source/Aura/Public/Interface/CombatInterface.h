// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** 获取玩家等级 */
	virtual int32 GetPlayerLevel() const { return 0; }

	/** 获取玩家攻击插槽位置 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLoc);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	/** 死亡接口 */
	virtual void Die() = 0;
};
