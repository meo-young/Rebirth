#include "CharacterBase.h"

#include "AIHelpers.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "KismetTraceUtils.h"
#include "AI/AICharacter.h"
#include "Camera/CameraComponent.h"
#include "Component/InteractionComponent.h"
#include "Components/SpotLightComponent.h"
#include "Define/Define.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

	if (!bIsCanMove) return;

	SpotLightTracing();
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
	if (!bIsCanMove) return;
	
	// 입력 값을 2D 벡터로 변환합니다.
	const FVector2D& MoveVector = Value.Get<FVector2D>();

	// 캐릭터를 전방으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveVector.X))
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MoveVector.X);
	}

	// 캐릭터를 측면으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveVector.Y))
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MoveVector.Y);
	}
}

void ACharacterBase::DoInteract(const FInputActionValue& Value)
{
	if (!bIsCanMove) return;

	// 입력 값을 bool 값으로 변환합니다.
	InteractionComponent->StartInteraction();
}

void ACharacterBase::SpotLightTracing()
{
	if (!SpotLight) return;

	// 1) SpotLight 기준 파라미터 설정
	const FVector  Start          = SpotLight->GetComponentLocation();
	const FRotator Direction      = SpotLight->GetComponentRotation();
	const float    ConeHeight     = SpotLight->AttenuationRadius;   // 콘 높이 = 반경
	const float    ConeHalfAngle  = SpotLight->OuterConeAngle;      // UE SpotLight 각도는 반각(half-angle)

	// 2) 무시할 액터/컴포넌트 (자기 자신 및 손잡이/랜턴 등)
	const TArray<AActor*> ActorsToIgnore;

	// 3) ConeTrace 실행
	TArray<FHitResult> OutHits;
	const bool bHit = ConeTraceMulti(
		/*WorldContextObject*/ this,
		/*Start*/              Start,
		/*Direction*/          Direction,
		/*ConeHeight*/         ConeHeight,
		/*ConeHalfAngle*/      ConeHalfAngle,
		/*TraceChannel*/       UEngineTypes::ConvertToTraceType(ECC_MONSTER), // 필요시 커스텀 채널로 교체
		/*bTraceComplex*/      false,
		/*ActorsToIgnore*/     ActorsToIgnore,
		/*DrawDebugType*/      EDrawDebugTrace::ForOneFrame,
		/*OutHits*/            OutHits,
		/*bIgnoreSelf*/        true,
		/*TraceColor*/         FLinearColor::Yellow,
		/*TraceHitColor*/      FLinearColor::Green,
		/*DrawTime*/           0.05f
	);

	// 이번 프레임에 SpotLight에 포착된 AI 집합
	TSet<TWeakObjectPtr<AAICharacter>> CurrSpotlightHitSet;
	for (const FHitResult& Hit : OutHits)
	{
		if (AAICharacter* AI = Cast<AAICharacter>(Hit.GetActor()))
		{
			CurrSpotlightHitSet.Add(AI);
		}
	}

	// 1) 벗어난 대상: Prev - Curr
	{
		TSet<TWeakObjectPtr<AAICharacter>> ExitedSet = PrevSpotlightHitSet;
		ExitedSet = ExitedSet.Difference(CurrSpotlightHitSet);

		for (const TWeakObjectPtr<AAICharacter>& WeakAI : ExitedSet)
		{
			if (AAICharacter* AI = WeakAI.Get())
			{
				UE_LOG(LogTemp, Log, TEXT("[SpotLight] %s 가(이) 빛에서 벗어남"), *AI->GetName());
				AI->ExitDamagedState();
				// 필요 시 상태 리셋/타이머 초기화 등도 여기서 처리
			}
		}
	}

	// 다음 프레임 비교를 위해 Prev 업데이트
	PrevSpotlightHitSet = MoveTemp(CurrSpotlightHitSet);
}

bool ACharacterBase::ConeTraceMulti(const UObject* WorldContextObject, const FVector Start, const FRotator Direction,
	float ConeHeight, float ConeHalfAngle, ETraceTypeQuery TraceChannel, bool bTraceComplex,
	const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits,
	bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	OutHits.Reset();
 
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
 
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}
 
	TArray<FHitResult> TempHitResults;
	const FVector End = Start + (Direction.Vector() * ConeHeight);
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);
	// r = h * tan(theta / 2)
	const double ConeBaseRadius = ConeHeight * tan(ConeHalfAngleRad);
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);
 
	// Perform a sweep encompassing an imaginary cone.
	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);
 
	// Filter for hits that would be inside the cone.
	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		// theta = arccos((A • B) / (|A|*|B|)). |A|*|B| = 1 because A and B are unit vectors.
		const double DeltaAngle = FMath::Acos(Dot);
 
		// Hit is outside the angle of the cone.
		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}
 
		const double Distance = (HitResult.ImpactPoint - Start).Length();
		// Hypotenuse = adjacent / cos(theta)
		const double LengthAtAngle = ConeHeight / cos(DeltaAngle);
 
		// Hit is beyond the cone. This can happen because we sweep with spheres, which results in a cap at the end of the sweep.
		if (Distance > LengthAtAngle)
		{
			continue;
		}
 
		OutHits.Add(HitResult);
	}
 
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		// Cone trace.
		const double ConeSlantHeight = FMath::Sqrt((ConeBaseRadius * ConeBaseRadius) + (ConeHeight * ConeHeight)); // s = sqrt(r^2 + h^2)
		//DrawDebugCone(World, Start, Direction.Vector(), ConeSlantHeight, ConeHalfAngleRad, ConeHalfAngleRad, 32, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
 
		// Uncomment to see the trace we're actually performing.
		// DrawDebugSweptSphere(World, Start, End, ConeBaseRadius, TraceColor.ToFColor(true), (DrawDebugType == EDrawDebugTrace::Persistent), DrawTime);
 
		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			//DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.ImpactPoint, DrawDebugType, true, Hit, TraceHitColor, TraceHitColor, DrawTime);

			if (AActor* Actor = Hit.GetActor())
			{
				if (AAICharacter* AICharacter = Cast<AAICharacter>(Actor))
				{
					AICharacter->ApplyDamagedState();
				}
			}
		}
	}
 
	return (OutHits.Num() > 0);
}
