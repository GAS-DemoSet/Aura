#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	SetReplicates(true);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* LocalInputSub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(LocalInputSub);
	LocalInputSub->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
