#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

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
	virtual void InitializePrimaryAttributes() const;

	//~ Begin ActorInterface
	virtual void BeginPlay() override;
	//~ Begin EndInterface
	
protected:
	/** 武器模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/** ASC */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	/** ASC 属性集 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Ability")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	/** AS 参数默认属性值 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
};
