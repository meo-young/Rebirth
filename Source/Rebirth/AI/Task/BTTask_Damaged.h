#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Damaged.generated.h"

UCLASS()
class REBIRTH_API UBTTask_Damaged : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Damaged();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual  void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float AccumulatedTime = 0.0f;
	uint8 bIsKnocked : 1 = false;
	
};
