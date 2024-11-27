// 


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interface/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer || GetWorld() == nullptr) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotator.Pitch = 0.f;

		FTransform SpawnTransform;
		// SpawnTransform.SetLocation(SocketLocation + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 100.f);
		SpawnTransform.SetLocation(SocketLocation);
		// TODO:: 设置射弹旋转器。
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
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

			for (const auto& Iter : DamageTypes)
			{
				const float TagValue = Iter.Value.GetValueAtLevel(GetAbilityLevel());
				// Set by caller 
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Iter.Key, TagValue);
			}
			
			Projectile->DamageEffectHandle = SpecHandle;
		}
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}
