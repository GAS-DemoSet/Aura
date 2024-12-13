#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	if (InTargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	// 玩家
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);
	if (TargetASC == nullptr)
		return;

	check(InGameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(InGameplayEffectClass,  ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); //应用到玩家身上

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolity == EEffectRemovalPolity::RemoveEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void AAuraEffectActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
}

void AAuraEffectActor::OnOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstanceEffectApplicationPolity == EEffectApplicationPolity::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolity == EEffectApplicationPolity::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolity == EEffectApplicationPolity::ApplyOnOverlap)
	{
		ApplyEffectToTarget(OtherActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstanceEffectApplicationPolity == EEffectApplicationPolity::ApplyEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolity == EEffectApplicationPolity::ApplyEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolity == EEffectApplicationPolity::ApplyEndOverlap)
	{
		ApplyEffectToTarget(OtherActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolity == EEffectRemovalPolity::RemoveEndOverlap)
	{
		// 玩家
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (!TargetASC)
			return;

		for (auto Iter = ActiveEffectHandles.CreateIterator(); Iter; ++Iter)
		{
			if (Iter->Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(Iter.Key(), 1);
				Iter.RemoveCurrent();
			}
		}
	}
}
