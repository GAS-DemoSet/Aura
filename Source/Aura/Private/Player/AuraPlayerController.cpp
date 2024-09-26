#include "Player/AuraPlayerController.h"

#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interface/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	SetReplicates(true);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 本地权威
	//if (GetLocalRole() == ROLE_AutonomousProxy)
	// 只有本地权威用户拥有增强输入组件
	// 程序开始 PlayerController 注册输入事件
	UEnhancedInputLocalPlayerSubsystem* LocalInputSub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (LocalInputSub)
	{
		check(AuraContext);
		LocalInputSub->AddMappingContext(AuraContext, 0);

		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* EnhancedInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

	// BindAbilityActions中实际做的事情就是将下面的函数模板化，并且将事先准备好的标签传入
	// EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputTagPressed, FAuraGameplayTags::Get()->Attributes_Primary_Intelligence);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighLightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighLightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
		}
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InVal)
{
	const FVector2D InputAxisValue = InVal.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator(0.f, Rotator.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (APawn* TempPawn = GetPawn())
	{
		TempPawn->AddMovementInput(ForwardDir, InputAxisValue.Y);
		TempPawn->AddMovementInput(RightDir, InputAxisValue.X);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(const FInputActionValue& InVal, FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Black, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, *InputTag.ToString());
}
