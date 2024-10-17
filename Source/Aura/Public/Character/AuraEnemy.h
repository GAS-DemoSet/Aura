

#pragma once

#include "CoreMinimal.h"
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
	//~ End AAuraCharacterBase Interface

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bHighLightDebug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;
};
