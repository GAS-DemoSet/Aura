// 

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UAuraInputComponent();

	/**  */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
	{
		checkf(InputConfig, TEXT("error: BindAbilityActions:: InputConfig null!!!"));

		for (auto& Iter: InputConfig->AbilityInputActions)
		{
			if (Iter.InputAction && Iter.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindAction(Iter.InputAction, ETriggerEvent::Started, Object, PressedFunc, Iter.InputTag);
				}
				if (ReleasedFunc)
				{
					BindAction(Iter.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Iter.InputTag);
				}
				if (HeldFunc)
				{
					BindAction(Iter.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Iter.InputTag);
				}
			}
		}
	}

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
