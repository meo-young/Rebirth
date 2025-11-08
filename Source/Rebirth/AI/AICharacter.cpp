#include "AI/AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enum/ESFX.h"
#include "Library/SoundLibrary.h"


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

void AAICharacter::PlayDamagedMontage()
{
	USoundLibrary::PlaySFXInLocation(GetMesh(), ESFX::Monster_Damaged, GetActorLocation());
	GetMesh()->GetAnimInstance()->Montage_Play(DamagedMontage);
	SubMesh1->GetAnimInstance()->Montage_Play(SubDamagedMontage1);
	SubMesh2->GetAnimInstance()->Montage_Play(SubDamagedMontage2);
}
