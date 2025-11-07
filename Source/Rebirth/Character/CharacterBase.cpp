#include "CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Component/InteractionComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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

	// Interaction Component를 초기화합니다.
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// 랜턴 손잡이를 생성합니다.
	LanternKnob = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	LanternKnob->SetupAttachment(GetMesh(), FName(TEXT("RightHand_end")));

	// Physics Constraint의 Target이 되는 랜턴을 생성합니다.
	TargetLantern = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetLantern"));
	TargetLantern->SetupAttachment(LanternKnob, FName(TEXT("Head")));
	TargetLantern->SetCollisionProfileName(FName("OverlapAll"));
	TargetLantern->SetHiddenInGame(true);
	TargetLantern->SetVisibility(false);

	// Physics Constraint의 Root가 되는 랜턴을 생성합니다.
	RootLantern = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootLantern"));
	RootLantern->SetupAttachment(LanternKnob, FName(TEXT("Head")));
	RootLantern->SetSimulatePhysics(true);
	
	// Constraint 컴포넌트는 Knob 쪽(Head 소켓)에 달아둠
	LanternJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LanternJoint"));
	LanternJoint->SetupAttachment(RootLantern);

	// 랜턴 라이트를 초기화합니다.
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootLantern);
	SpotLight->Intensity = 4000.f;
	SpotLight->AttenuationRadius = 1000.f;
	SpotLight->InnerConeAngle = 30.f;
	SpotLight->OuterConeAngle = 40.f;
	SpotLight->SetRelativeLocation(FVector(0.f, 0.f, -17.f));
	SpotLight->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	SpotLight->CastShadows = true;
	SpotLight->VolumetricScatteringIntensity = 30.0f;
	
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
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

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
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

void ACharacterBase::DoInteract(const FInputActionValue& Value)
{
	// 입력 값을 bool 값으로 변환합니다.
	InteractionComponent->StartInteraction();
}
