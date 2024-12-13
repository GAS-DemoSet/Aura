#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolity : uint8
{
	ApplyOnOverlap,
	ApplyEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolity : uint8
{
	RemoveEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass);

protected:
	virtual void BeginPlay() override;

	//~ Begin AActor Interface
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	//~ End AActor Interface

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OtherActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OtherActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects")
	bool bApplyEffectsToEnemies = false;

	/** 效果等级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects")
	float ActorLevel = 1.0f;
	
	/** 立即执行一次效果 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Instance")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Instance")
	EEffectApplicationPolity InstanceEffectApplicationPolity = EEffectApplicationPolity::DoNotApply;

	/** 持续执行一定的量级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Duration")
	EEffectApplicationPolity DurationEffectApplicationPolity = EEffectApplicationPolity::DoNotApply;

	/** 将无限执行下去（血泉） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Infinite")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Infinite")
	EEffectApplicationPolity InfiniteEffectApplicationPolity = EEffectApplicationPolity::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appleid Effects|Infinite")
	EEffectRemovalPolity InfiniteEffectRemovalPolity = EEffectRemovalPolity::RemoveEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
