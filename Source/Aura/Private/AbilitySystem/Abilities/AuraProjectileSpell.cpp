// 


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interface/CombatInterface.h"
#include "Log/AuraLog.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer || GetWorld() == nullptr) return;

	// TODO:: 有空把这个插槽对应得蓝图和动画文件里得通知标签都修改了，动画资产为：AM_AbilityMontage，GA资产为：GA_FireBolt， 角色资产为：BP_AuraCharacter。
	// const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get()->CombatSocket_Weapon);
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
	// Rotator.Pitch = 0.f;

	FTransform SpawnTransform;
	// SpawnTransform.SetLocation(SocketLocation + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 100.f);
	SpawnTransform.SetLocation(SocketLocation);
	// TODO:: 设置射弹旋转器。
	SpawnTransform.SetRotation(Rotator.Quaternion());
	
	
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Print_Log_NetRole(GetAvatarActorFromActorInfo(), Warning, *FString::Printf(TEXT("开始生成火焰投射物")));
	
	// TODO:: 为射弹物提供造成伤害的 Gameplay Effect 规格。
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (SourceASC)
	{
		FGameplayEffectContextHandle GameplayEffectContextHandle = SourceASC->MakeEffectContext();
		GameplayEffectContextHandle.SetAbility(this);
		GameplayEffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		GameplayEffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		GameplayEffectContextHandle.AddHitResult(HitResult);
		
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), GameplayEffectContextHandle);

		// 注册伤害类型标签值
		for (const auto& Iter : DamageTypes)
		{
			const float TagValue = Iter.Value.GetValueAtLevel(GetAbilityLevel());
			// Set by caller 
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Iter.Key, TagValue);
		}
		
		Projectile->DamageEffectHandle = SpecHandle;
		Print_Log_NetRole(GetAvatarActorFromActorInfo(), Warning, *FString::Printf(TEXT("完成设置火焰投射物参数")));
	}
	
	Projectile->FinishSpawning(SpawnTransform);
	Print_Log_NetRole(GetAvatarActorFromActorInfo(), Warning, *FString::Printf(TEXT("结束生成火焰投射物")));
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}
