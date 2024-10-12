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
		if (bTargeting) // 点中得是敌方目标
		{
			if (GetASC())
			{
				GetASC()->AbilityInputTagReleased(InputTag);
			}
		}
		else
		{
			APawn* ControlledPawn = GetPawn();
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
						DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
					}
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
		if (bTargeting) // 点中得是敌方目标
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

			FHitResult Hit;
			if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
			{
				CachedDestination = Hit.ImpactPoint;
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
