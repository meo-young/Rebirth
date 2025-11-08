#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

UCLASS()
class REBIRTH_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
