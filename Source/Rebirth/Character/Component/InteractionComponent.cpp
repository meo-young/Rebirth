#include "Character/Component/InteractionComponent.h"
#include "Rebirth.h"
#include "Interface/Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Start = Owner->GetActorLocation();
	FVector End = Start + Owner->GetActorForwardVector() * 200.f; 
	float Radius = 50.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner); 

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	// 디버그용 구체 표시
	DrawDebugSphere(GetWorld(), End, Radius, 16, bHit ? FColor::Green : FColor::Red, false, 0.1f);

	if (bHit && HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}
}

void UInteractionComponent::StartInteraction()
{
	LOG(TEXT("Interaction 호출"));
	
	// 상호작용 오브젝트 캐스팅
	//InteractableObject = Cast<IInteractable>(InteractionHitResult.GetActor());
	

	// 상호작용 시작 델리게이트 호출
	if (OnInteractionStartedDelegate.IsBound())
	{
		LOG(TEXT("호출"));
		OnInteractionStartedDelegate.Broadcast();
	}
}

