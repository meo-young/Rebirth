#include "Character/Component/InteractionComponent.h"
#include "Rebirth.h"
#include "Character/CharacterBase.h"
#include "Character/PlayerControllerBase.h"
#include "Define/Define.h"
#include "Interface/Interactable.h"
#include "UI/GuideWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = GetOwner();
	if (!Owner) return;

	QueryParams.AddIgnoredActor(Owner); 
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const ACharacterBase* Owner = Cast<ACharacterBase>(GetOwner());
	if (!Owner) return;
	
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector Start = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - 40.0f);
	const FVector End = Start + Owner->GetActorForwardVector() * 80.f; 
	constexpr float Radius = 50.f;
	
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_INTERACTION,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);
	

	// 디버그용 구체 표시
	//DrawDebugSphere(GetWorld(), End, Radius, 16, bHit ? FColor::Green : FColor::Red, false, 0.1f);
	
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_ShowInteractGuide(HitActor);
		}
	}
	else
	{
		if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(Owner->Controller))
		{
			if (UGuideWidget* GuideWidget = PlayerController->GuideWidgetInstance)
			{
				GuideWidget->HideGuide();
			}
		}
	}
}

void UInteractionComponent::StartInteraction()
{
	LOG(TEXT("Interaction 호출"));
	
	// 상호작용 오브젝트 캐스팅합니다.
	InteractableObject = Cast<IInteractable>(HitResult.GetActor());
	
	// 상호작용 오브젝트가 유효하다면 상호작용 함수 호출합니다.
	if (InteractableObject)
	{
		IInteractable::Execute_Interact(InteractableObject->_getUObject());

		// 상호작용 시작 델리게이트 호출합니다.
		if (OnInteractionStartedDelegate.IsBound())
		{
			LOG(TEXT("호출"));
			OnInteractionStartedDelegate.Broadcast();
		}
	}
	else
	{
		LOG(TEXT("Interactable 인터페이스가 구현되지 않은 오브젝트입니다."));
	}
}

