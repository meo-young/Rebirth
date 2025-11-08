#include "Animation/Notify/AnimNotify_FootStep.h"
#include "Rebirth.h"
#include "Define/Define.h"
#include "Enum/ESFX.h"
#include "Library/SoundLibrary.h"
#include "SubSystem/SoundSubSystem.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	// Notify를 호출한 메시의 소유자를 가져온다.
	const AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	const UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) return;	

	// PhysicalMaterial을 가져오기 위한 쿼리 파라미터를 설정한다.
	QueryParams.bReturnPhysicalMaterial = true;
	
	// 액터의 -150.0f 아래 위치를 계산한다.
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector DownVector = Owner->GetActorUpVector() * -200.0f;
	const FVector TargetLocation = CurrentLocation + DownVector;

	// LineTrace 실시한다.
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		CurrentLocation,
		TargetLocation,
		ECC_Visibility,
		QueryParams
		);

#if WITH_EDITOR
	// 라인트레이스 시각화
	const FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(
		World,
		CurrentLocation,
		TargetLocation,
		LineColor,
		false,      // 지속 시간 후 자동 제거
		1.5f,       // 표시 시간 (초)
		0,
		2.0f        // 두께
	);
#endif
	
	// Surface Type에 따른 발걸음 소리를 재생한다.
	if (bHit)
	{
		if (HitResult.PhysMaterial.IsValid())
		{
			switch (HitResult.PhysMaterial->SurfaceType)
			{
			case SURFACE_GRASS:
				LOG(TEXT("Grass"));
				USoundLibrary::PlaySFXInLocation(Owner, ESFX::Footstep_Grass, TargetLocation);
				break;
			case SURFACE_GROUND:
				LOG(TEXT("Ground"));
				USoundLibrary::PlaySFXInLocation(Owner, ESFX::Footstep_Ground, TargetLocation);
				break;
			default:
				LOG(TEXT("Default"));
				USoundLibrary::PlaySFXInLocation(Owner, ESFX::Footstep_Grass, TargetLocation);
				break;
			}
		}
	}
}