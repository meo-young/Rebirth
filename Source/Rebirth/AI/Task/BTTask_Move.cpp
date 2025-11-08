#include "AI/Task/BTTask_Move.h"
#include "Rebirth.h"
#include "AI/AICharacter.h"
#include "AI/AIControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Move::UBTTask_Move()
{
	bCreateNodeInstance = true;
	NodeName = "Move";
	bNotifyTick = true;
	bNotifyTaskFinished = false;
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());
	if (!AI) return EBTNodeResult::Failed;

	AIController->StopMovement();

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		AIController->MoveTo(Player);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());
	if (!AI) return;

	if (const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		const FVector PlayerLocation = Player->GetActorLocation();
		const FVector AILocation = AI->GetActorLocation();
		const float Distance = FVector::Dist(PlayerLocation, AILocation);

		if (Distance < 125.0f)
		{
			LOG(TEXT("거리가 충분히 가까워졌습니다"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}