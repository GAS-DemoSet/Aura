#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextWidgetComponent;
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

	UFUNCTION(Blueprintable)
	bool IsShiftKeyDown() const { return bShiftKeyDown; }

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	/** 鼠标检测是否在怪物身上 */
	void CursorTrace();

private:
	void Move(const FInputActionValue& InVal);

	void ShiftPressed();
	void ShiftReleased();

	/** 左键单机地板后自行移动 */
	void AutoRun();

	void AbilityInputTagPressed(const FInputActionValue& InVal, FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DamageText")
	TSubclassOf<UDamageTextWidgetComponent> DamageTextClass;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAuraAbilitySystemComponent> ASC = nullptr;

	IEnemyInterface* LastActor = nullptr;
	IEnemyInterface* ThisActor = nullptr;
	FHitResult CursorHitResult;

	/** 缓存目的地位置 */
	FVector CachedDestination = FVector::ZeroVector;
	/** 鼠标按下时长（角色跟随鼠标移动） */
	float FollowTime = 0.f;
	/** 短按阈值，通过此值界定长按或者短按 */
	float ShortPressThreshold = 0.5f;
	/** 自行移动 */
	bool bAutoRunning = false;
	/** 鼠标触碰到得是否为目标（敌方目标） */
	bool bTargeting = false;
	/** 导航可接受得误差半径 */
	UPROPERTY(EditDefaultsOnly)
	float AutRunAcceptanceRadius = 50.f;
	/** 导航线路 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	bool bShiftKeyDown = false; 
};
