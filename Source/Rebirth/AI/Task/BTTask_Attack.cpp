#include "AI/Task/BTTask_Attack.h"
#include "Rebirth.h"
#include "AI/AICharacter.h"
#include "AI/AIControllerBase.h"

UBTTask_Attack::UBTTask_Attack()
{
	bCreateNodeInstance = true;
	NodeName = "Attack";
	bNotifyTick = true;
	bNotifyTaskFinished = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());

	AIController->StopMovement();

	LOG(TEXT("Attack 상태"));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());
}
