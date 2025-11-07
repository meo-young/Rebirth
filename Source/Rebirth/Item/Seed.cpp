#include "Item/Seed.h"
#include "Rebirth.h"
#include "Define/Define.h"

ASeed::ASeed()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트를 생성합니다.
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	// 씨앗 메시를 생성합니다.
	SeedMesh = CreateDefaultSubobject<UStaticMeshComponent>("SeedMesh");
	SeedMesh->SetupAttachment(Root);
	SeedMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SeedMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	SeedMesh->SetCollisionResponseToChannel(ECC_INTERACTION, ECollisionResponse::ECR_Block);
}

void ASeed::BeginPlay()
{
	Super::BeginPlay();
}

void ASeed::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	LOG(TEXT("호출"));
}


