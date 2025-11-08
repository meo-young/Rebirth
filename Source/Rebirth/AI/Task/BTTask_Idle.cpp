#include "AI/Task/BTTask_Idle.h"
#include "Rebirth.h"
#include "AI/AICharacter.h"
#include "AI/AIControllerBase.h"

UBTTask_Idle::UBTTask_Idle()
{
	bCreateNodeInstance = true;
	NodeName = "Idle";
	bNotifyTick = true;
	bNotifyTaskFinished = false;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());

	AIController->StopMovement();

	AccumulatedTime = 0.0f;

	LOG(TEXT("Idle 상태"));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());

	AccumulatedTime += DeltaSeconds;
	if (AccumulatedTime > 1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
