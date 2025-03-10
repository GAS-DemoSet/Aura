﻿// 


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Log/AuraLog.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicatingMovement(true);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 投射组件
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		UE_LOG(AuraLog, Warning, TEXT("AAuraProjectile::BeginPlay(): %s"), *GetOwner()->GetName());
	}

	SetReplicateMovement(true);

	SetLifeSpan(LifeSpan);
	

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	// 在客服端如果没有碰撞目标即被删除时主动调用特效与播放音效
	if (!bHit && !HasAuthority())
	{
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
		
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	Print_Log_NetRole(GetInstigator(), Warning, *FString::Printf(TEXT("火焰投射物被移除了")))
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Print_Log_NetRole(GetInstigator(), Warning, *FString::Printf(TEXT("火焰投射物碰到了{%s--%s}"), *OtherActor->GetName(), *GetInstigator()->GetName()));
	if (!GetInstigator() || GetInstigator() == OtherActor ) return;

	/**
	 * DamageEffectHandle.Data.Get()->GetContext().GetEffectCauser() 获取得对象和 GetInstigator() 是同一个
	 * 通过 SourceASC->MakeEffectContext() 创建中直接将 ASC 得 AvatarActor 赋值给 FGameplayEffectContextHandle
	 */ 
	// if (!UAuraAbilitySystemLibrary::IsNotFriend(DamageEffectHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	if (!UAuraAbilitySystemLibrary::IsNotFriend(GetInstigator(), OtherActor))
	{
		return;
	}

	if (!bHit)
	{
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
	
		// 播放击中声音
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		// 生成击中特效
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	
	if (HasAuthority())
	{
		// 获取到被击中目标的 ASC 
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC)
		{
			if (ensureMsgf(DamageEffectHandle.IsValid(), TEXT("AAuraProjectile::OnSphereOverlap:: DanageEffectHandle is null!!!")))
			{
				// 应用效果参数（伤害值）
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data);
			}
		}
		// 由服务器调用删除
		Destroy(true);
		Print_Log_NetRole(GetInstigator(), Warning, *FString::Printf(TEXT("删除火焰投射物")))
	}
	else
	{
		// 在网络延迟得情况下，可能导致客户端在没有碰到目标时而被服务器强制干掉
		bHit = true;
	}
}

