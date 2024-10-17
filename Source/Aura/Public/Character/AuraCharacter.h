#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	//~ End APawn Interface

	//~ Begin ICombatInterface
	virtual int32 GetPlayerLevel()  const override;
	//~ End ICombatInterface

private:
	//~ Begin AAuraCharacterBase Interface
	virtual void InitAbilityActorInfo() override;
	//~ End AAuraCharacterBase Interface
};
