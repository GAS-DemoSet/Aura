// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

private:
	/** 投射物存活时间 */
	UPROPERTY(EditAnywhere)
	float LifeSpan = 15.f;
	
	/** 防止客户端与服务器重叠 */
	bool bHit = false;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	/** 击中特效 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	/** 击中音效 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	/** 投射过程中的音效 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	/** 在投射过程中产生，临时存储对象 */
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
