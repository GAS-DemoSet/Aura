// 


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 网络更新频率
	NetUpdateFrequency = 100.f;

	// 构建 ASC 组件
	AuraAbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AuraAbilitySystemComponent->SetIsReplicated(true);
	AuraAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 构建 AS 属性集
	AuraAttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

UAuraAbilitySystemComponent* AAuraPlayerState::GetAuraAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* AAuraPlayerState::GetAuraAttributeSet() const
{
	return AuraAttributeSet;
}

void AAuraPlayerState::OnRep_Level(int32 OldValue)
{
}
