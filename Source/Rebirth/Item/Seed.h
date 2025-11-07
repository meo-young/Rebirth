#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Seed.generated.h"

UCLASS()
class REBIRTH_API ASeed : public AItemBase
{
	GENERATED_BODY()

public:
	ASeed();

public:
	/** Interactable 인터페이스의 Interact 함수 구현 */
	virtual void Interact_Implementation() override;
	virtual void ShowInteractGuide_Implementation() override;
	
};
