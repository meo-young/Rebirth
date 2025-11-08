#include "AI/AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


AAICharacter::AAICharacter()
{
	SubMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>("SubMesh1");
	SubMesh1->SetupAttachment(GetMesh());

	SubMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>("SubMesh2");
	SubMesh2->SetupAttachment(GetMesh());
}

void AAICharacter::ApplyDamagedState()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", true);
}

void AAICharacter::ExitDamagedState()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	AIController->GetBlackboardComponent()->SetValueAsBool("IsDamaged", false);
}
