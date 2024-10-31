

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlapWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
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
	//~ End ICombatInterface

protected:
	//~ Begin IEnemyInterface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	//~ End IEnemyInterface

	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	//~ End AAuraCharacterBase Interface

	virtual void BeginPlay() override;

private:
	void InitAndBindAttributeChanged();

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
};
