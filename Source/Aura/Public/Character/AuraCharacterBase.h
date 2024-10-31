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

	//~ Begin ICombatInterface
	virtual FVector GetCombatSocketLocation() const override;
	//~ End ICombatInterface
	
protected:
	/** 武器模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponTipSocketName = NAME_None;

	/** ASC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/** ASC 属性集 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	/** AS 参数默认属性值 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	/** AS 参数默认属性值 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	/** AS 参数默认属性值 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
