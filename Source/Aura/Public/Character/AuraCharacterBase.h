#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;
class UMaterialInstanceDynamic;
class UMaterialInstance;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase
	: public ACharacter
	, public IAbilitySystemInterface
	, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	
	USkeletalMeshComponent* GetWeaponMesh() const;

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface

	//~ Begin ICombatInterface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation() const override;
    virtual bool IsDead_Implementation() const override;
    virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	//~ End ICombatInterface
	
	UAuraAttributeSet* GetAuraAttributeSet() const;

protected:
	/** 初始化 ASC 信息 */
	virtual void InitAbilityActorInfo();

	/** 初始化 AS 属性 */
	virtual void InitializeDefaultAttributes() const;

	/** 应用 GE 效果到自身 */
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GE, float Level = 1.f) const;

	/** 添加玩家能力集 GA */
	void AddCharacterAbilities();
	
	//~ Begin ActorInterface
	virtual void BeginPlay() override;
	//~ Begin EndInterface

	UFUNCTION(NetMulticast, Reliable)
	virtual void EventHandleDeath_Multicast();

	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent, Category="Dissolve")
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMatIns, UMaterialInstanceDynamic* WeaponDynamicMatIns);
	
protected:
	/** 已死亡 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bDead;
	
	/** 武器模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** 攻击插槽，这个插槽在武器上设置 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponTipSocketName = NAME_None;

	/** ASC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/** ASC 属性集 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	/** AS 参数默认属性值（初始化使用） 主要属性 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	/** AS 参数默认属性值（初始化使用） 次要属性 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	/** AS 参数默认属性值（初始化使用） 重要属性 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/** 溶解效果材质(死亡后溶解) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

private:
	/** 玩家自带技能，游戏初始化阶段进行注册 */
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** 玩家受击蒙太奇 */
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
