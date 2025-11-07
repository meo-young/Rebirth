#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UInputMappingContext;

UCLASS()
class REBIRTH_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> MappingContext;
	
};
