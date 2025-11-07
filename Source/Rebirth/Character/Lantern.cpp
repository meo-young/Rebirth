#include "Character/Lantern.h"
#include "Components/SpotLightComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ALantern::ALantern()
{
	// ALantern::ALantern()

	// Root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Knob : 손에 붙는 고정 핸들(비시뮬레이트, 충돌만)
	LanternKnob = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternKnob"));
	LanternKnob->SetupAttachment(Root);
	LanternKnob->SetSimulatePhysics(false);
	LanternKnob->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LanternKnob->SetGenerateOverlapEvents(false);

	// Lantern : 실제로 흔들릴 본체(시뮬레이트)
	Lantern = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern"));
	Lantern->SetupAttachment(Root);               // <- 형제로 둠
	Lantern->SetSimulatePhysics(true);
	Lantern->SetEnableGravity(true);
	Lantern->SetLinearDamping(0.2f);
	Lantern->SetAngularDamping(2.5f);
	Lantern->BodyInstance.bUseCCD = true;         // 관통 방지
	Lantern->SetCollisionObjectType(ECC_PhysicsBody);

	// Constraint : 두 컴포넌트를 연결하는 관절
	LanternJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LanternJoint"));
	LanternJoint->SetupAttachment(LanternKnob);

	// 라이트(옵션)
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(Lantern);
	SpotLight->Intensity = 5000.f;
	SpotLight->AttenuationRadius = 800.f;
	SpotLight->InnerConeAngle = 20.f;
	SpotLight->OuterConeAngle = 40.f;
	SpotLight->SetRelativeLocation(FVector(0.f, 0.f, -5.f));
	SpotLight->CastShadows = true;


	
}

void ALantern::BeginPlay()
{
	Super::BeginPlay();

	// 관절 기준점을 Knob 위치로 맞춰두면 안정적
	LanternJoint->SetWorldLocation(LanternKnob->GetComponentLocation());

	// 두 바디를 관절로 연결
	LanternJoint->SetConstrainedComponents(
		LanternKnob, NAME_None,
		Lantern,     NAME_None
	);

	
}



