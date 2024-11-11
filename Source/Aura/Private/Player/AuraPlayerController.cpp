#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interface/EnemyInterface.h"
#include "Log/AuraLog.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextWidgetComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	SetReplicates(true);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(GetRootComponent());
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!ASC.IsValid())
	{
		ASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ASC.Get();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if (TargetCharacter && DamageTextClass.GetGCPtr())
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
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
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

	// BindAbilityActions中实际做的事情就是将下面的函数模板化，并且将事先准备好的标签传入
	// EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputTagPressed, FAuraGameplayTags::Get()->Attributes_Primary_Intelligence);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (ThisActor) ThisActor->HighLightActor();
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

void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyDown = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyDown = false;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(DirectionOnSpline);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(const FInputActionValue& InVal, FGameplayTag InputTag)
{
	//GetASC()->AbilityInputTagPressed(InputTag);
	UE_LOG(AuraLog, Warning, TEXT("AAuraPlayerController::AbilityInputTagPressed:: [%s]"), *InputTag.ToString());

	// 鼠标左键事件
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get()->InputTag_LMB))
	{
		// 点中得敌方目标
		bTargeting = ThisActor ? true : false;
		
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UE_LOG(AuraLog, Warning, TEXT("AAuraPlayerController::AbilityInputTagReleased:: [%s]"), *InputTag.ToString());

	// 鼠标左键事件
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get()->InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		if (!bTargeting && !IsShiftKeyDown()) // 点中得是敌方目标
		{
			const APawn* ControlledPawn = GetPawn();
			if (FollowTime <= ShortPressThreshold && ControlledPawn)
			{
				// 可能会出现客户端运行无法生成问题
				// 1、检查是否铺设导航网格
				// 2、检查项目设置中是否开始允许客户端进行导航
				if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
						// DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
					}
					// 设置路径最后一个点为导航终点
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
			FollowTime = 0.f;
			bTargeting = false;
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UE_LOG(AuraLog, Warning, TEXT("AAuraPlayerController::AbilityInputTagHeld:: [%s]"), *InputTag.ToString());

	// 鼠标左键事件
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get()->InputTag_LMB))
	{
		if (bTargeting || IsShiftKeyDown()) // 点中得是敌方目标
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagHeld(InputTag);
			}
		}
		else
		{
			// 长按进行移动
			FollowTime += GetWorld()->GetDeltaSeconds();
			
			if (CursorHitResult.bBlockingHit)
			{
				CachedDestination = CursorHitResult.ImpactPoint;
			}

			if (APawn* ControlledPawn = GetPawn())
			{
				const FVector WorldDir = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
				ControlledPawn->AddMovementInput(WorldDir);
			}
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}
