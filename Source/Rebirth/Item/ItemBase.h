#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "ItemBase.generated.h"

UCLASS()
class REBIRTH_API AItemBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AItemBase();
	virtual void BeginPlay() override;

public:
	/** Interactable 인터페이스의 Interact 함수 구현 */
	virtual void Interact_Implementation() override;
	virtual void ShowInteractGuide_Implementation() override;

protected:
	/** 루트 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	/** 씨앗에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> SeedMesh;

	/** 상호작용이 가능할 때 화면에 표시할 Guide Text입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|가이드")
	FString GuideText;

};
