#include "Character/AuraEnemy.h"

#include "Aura.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "AI/AuraEnemyBlackboardParamName.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 构建 ASC 组件
	AuraAbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AuraAbilitySystemComponent->SetIsReplicated(true);
	AuraAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// 构建 AS 属性集
	AuraAttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetWeaponMesh()->SetRenderCustomDepth(true);
	GetWeaponMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetWeaponMesh()->SetRenderCustomDepth(false);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget.Get();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// 初始话 ASC 组件信息
	check(AuraAbilitySystemComponent != nullptr);
	AuraAbilitySystemComponent->InitAbilityActorInfo(this, this);
	AuraAbilitySystemComponent->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, GetPlayerLevel(), AuraAbilitySystemComponent);
}

int32 AAuraEnemy::GetPlayerLevel() const
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FAuraEnemyBlackboardParamName::Dead, true);
	}
	Super::Die();
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	check(AuraAIController);
	check(BehaviorTree);

	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);

	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FAuraEnemyBlackboardParamName::HitReacting, false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FAuraEnemyBlackboardParamName::RangedAttacker, CharacterClass != ECharacterClass::ECC_Warrior);
}

void AAuraEnemy::UnPossessed()
{
	Super::UnPossessed();
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();

	InitAndBindAttributeChanged();

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbility(this, CharacterClass, AuraAbilitySystemComponent);
	}
}

void AAuraEnemy::InitAndBindAttributeChanged()
{
	if (UAuraUserWidget* EnemyHealth = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		EnemyHealth->SetWidgetController(this);
	}

	if (AuraAttributeSet && AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		/** 注册 GameplayTags 事件，当 GameplayTag 增加或移除时通知, Effect_HitReact 标签增加或移除时调用 */
		AuraAbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get()->Effect_HitReact).AddUObject(this, &AAuraEnemy::EventOnHitReactTagChanged);

		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::EventOnHitReactTagChanged(const FGameplayTag GameplayTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FAuraEnemyBlackboardParamName::HitReacting, bHitReacting);
	}
}
