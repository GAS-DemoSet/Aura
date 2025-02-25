

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlapWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UWidgetComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//~ Begin ICombatInterface
	virtual int32 GetPlayerLevel() const override;
	virtual void Die() override;
	//~ End ICombatInterface

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	// virtual void OnRep_PlayerState() override;
	//~ End APawn Interface

	/** 当前是否为被打击状态 */
	bool IsHitReacting() const { return bHitReacting; }

protected:
	//~ Begin IEnemyInterface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	//~ End IEnemyInterface

	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	//~ End AAuraCharacterBase Interface

	virtual void BeginPlay() override;

private:
	void InitAndBindAttributeChanged();

	/** 当存在 Effect_HitReact 标签时调用 */
	void EventOnHitReactTagChanged(const FGameplayTag GameplayTag, int32 NewCount);

public:
	/** 当怪物血量变化时 */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	/** 当怪物最大血量变化时 */
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bHighLightDebug;

	/** 怪物等级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	/** 怪物类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::ECC_Elementalist;

	/** 怪物血条组件 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;

	/** 打击反应中（被击中后的动作） */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	/** 基础行走速度 */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	
	/** 判定死亡后存留时间 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AAuraAIController> AuraAIController;

	/** 攻击目标 */
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TWeakObjectPtr<AActor> CombatTarget;
};
