#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_Move.generated.h"

class AAICharacter;

UCLASS()
class REBIRTH_API UBTTask_Move : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Move();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
	
};
