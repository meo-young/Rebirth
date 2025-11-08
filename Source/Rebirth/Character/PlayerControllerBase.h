#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UFadeWidget;
class UGuideWidget;
class UInputMappingContext;

UCLASS()
class REBIRTH_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UGuideWidget> GuideWidgetInstance;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UFadeWidget> FadeWidgetInstance;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UGuideWidget> GuideWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UFadeWidget> FadeWidgetClass;
	
};
