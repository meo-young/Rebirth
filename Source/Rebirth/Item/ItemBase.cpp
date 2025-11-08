#include "Item/ItemBase.h"
#include "Rebirth.h"
#include "Character/PlayerControllerBase.h"
#include "Define/Define.h"
#include "UI/GuideWidget.h"

AItemBase::AItemBase()
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

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	SetActorEnableCollision(false);

	LOG(TEXT("호출"));
}

void AItemBase::ShowInteractGuide_Implementation()
{
	IInteractable::ShowInteractGuide_Implementation();

	if(APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		if (UGuideWidget* GuideWidget = PlayerController->GuideWidgetInstance)
		{
			GuideWidget->ShowGuide(GuideText);
		}
	}
}



