#include "AI/Task/BTTask_Damaged.h"
#include "Rebirth.h"
#include "AI/AICharacter.h"
#include "AI/AIControllerBase.h"
#include "Character/CameraManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Damaged::UBTTask_Damaged()
{
	bCreateNodeInstance = true;
	NodeName = "Damaged";
	bNotifyTick = true;
	bNotifyTaskFinished = false;
}

EBTNodeResult::Type UBTTask_Damaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());

	AIController->StopMovement();

	AccumulatedTime = 0.0f;
	bIsKnocked = false;

	LOG(TEXT("Damaged 상태"));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Damaged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	AAICharacter* AI = Cast<AAICharacter>(AIController->GetPawn());
	ACameraManager* CameraManager = Cast<ACameraManager>(UGameplayStatics::GetPlayerCameraManager(AI->GetWorld(), 0));

	AccumulatedTime += DeltaSeconds;
	
	// 2초 경과 시 1회 넉백 적용
	if (!bIsKnocked && AccumulatedTime >= AI->KnockbackThreshold)
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(AI->GetWorld(), 0);
		if (Player)
		{
			// 플레이어 -> AI 방향 단위벡터
			FVector Dir = (AI->GetActorLocation() - Player->GetActorLocation());
			Dir.Z = 0.0f; // 수평 넉백
			Dir = Dir.GetSafeNormal();

			const FVector KnockbackVelocity = Dir * AI->KnockbackScale;

			// LaunchCharacter로 즉시 속도 적용(수평/수직 모두 덮어쓰기)
			AI->LaunchCharacter(KnockbackVelocity, /*bXYOverride*/true, /*bZOverride*/true);

			UE_LOG(LogTemp, Log, TEXT("Damaged Task: Knockback applied away from Player."));

			AI->PlayDamagedMontage();

			
			CameraManager->StopShaking();
			CameraManager->ApplyShake(EShakeState::Monster_Damaged, AI->CameraShakeScale);

			bIsKnocked = true;
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		CameraManager->ApplyShake(EShakeState::Monster_Loop, AI->CameraShakeScale);
	}
}
