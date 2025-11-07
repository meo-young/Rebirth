#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractable;
DECLARE_MULTICAST_DELEGATE(FOnInteractionPossible);
DECLARE_MULTICAST_DELEGATE(FOnInteractionImPossible);
DECLARE_MULTICAST_DELEGATE(FOnInteractionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBIRTH_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** 상호작용이 가능한 상태일 때 호출되는 델리게이트 */
	FOnInteractionPossible OnInteractionPossibleDelegate;

	/** 상호작용이 불가능한 상태일 때 호출되는 델리게이트 */
	FOnInteractionImPossible OnInteractionImpossibleDelegate;

	/** 마우스를 클릭해서 상호작용이 시작될 때 호출되는 델리게이트 */
	FOnInteractionStarted OnInteractionStartedDelegate;


public:
	/** 마우스 클릭 시 호출되는 상호작용 시작 로직 함수 */
	void StartInteraction();

private:
	IInteractable* InteractableObject = nullptr;
	
	
};