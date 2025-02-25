

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AuraSetting.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UAuraSetting& Get();
	
	UFUNCTION(BlueprintPure, Category = "AuraSetting|Debug")
	static bool GetDrawDebug_GAS();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug Param")
	bool bDrawDebug_GAS;
};
