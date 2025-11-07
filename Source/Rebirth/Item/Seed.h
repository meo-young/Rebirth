#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Seed.generated.h"

UCLASS()
class REBIRTH_API ASeed : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	ASeed();
	virtual void BeginPlay() override;

public:
	/** Interactable 인터페이스의 Interact 함수 구현 */
	virtual void Interact_Implementation() override;

protected:
	/** 루트 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	/** 씨앗에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> SeedMesh;


};
