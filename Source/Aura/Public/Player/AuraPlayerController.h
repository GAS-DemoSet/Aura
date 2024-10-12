#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	UAuraAbilitySystemComponent* GetASC();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	/** 鼠标检测是否在怪物身上 */
	void CursorTrace();

private:
	void Move(const FInputActionValue& InVal);

	void AbilityInputTagPressed(const FInputActionValue& InVal, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAuraAbilitySystemComponent> ASC = nullptr;

	IEnemyInterface* LastActor = nullptr;
	IEnemyInterface* ThisActor = nullptr;

	/** 缓存目的地位置 */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	/** 短按阈值，通过此值界定长按或者短按 */
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;

	/** 鼠标触碰到得是否为目标（敌方目标） */
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
