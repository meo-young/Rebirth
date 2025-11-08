#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Idle.generated.h"

UCLASS()
class REBIRTH_API UBTTask_Idle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Idle();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float AccumulatedTime = 0.0f;
	
};
