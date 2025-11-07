#include "CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Lantern.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 를 초기화합니다.
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// SpringArmComponent를 초기화합니다.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 250.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 60.0f, 0.0f));

	// CameraComponent를 초기화합니다.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 130.0f), FRotator(-10.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = false;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (LanternClass)
	{
		Lantern = GetWorld()->SpawnActor<ALantern>(LanternClass, FTransform(GetActorLocation())); 

		Lantern->LanternKnob->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("LeftHand_16"));
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::DoMove);
		}
	}
}

void ACharacterBase::DoMove(const FInputActionValue& Value)
{
	// 입력 값을 2D 벡터로 변환합니다.
	const FVector2D& MoveVector = Value.Get<FVector2D>();

	// 카메라/컨트롤러 방향의 Yaw만 추출합니다.
	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

	// 방향 벡터를 계산합니다.
	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	// 캐릭터를 전방으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveVector.X))
	{
		AddMovementInput(Forward, MoveVector.X);
	}

	// 캐릭터를 측면으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveVector.Y))
	{
		AddMovementInput(Right, MoveVector.Y);
	}
}