#include "Character/CameraManager.h"

void ACameraManager::ApplyShake(const EShakeState ShakeState, const float Scale)
{
	if (bIsApplyLoopShake) return;
	
	bIsApplyLoopShake = true;
	
	switch (ShakeState)
	{
	case EShakeState::Monster_Loop:
		StartCameraShake(MonsterLoopShakeClass, Scale);
		break;
	case EShakeState::Monster_Damaged:
		StartCameraShake(MonsterDamagedShakeClass, Scale);
		GetWorld()->GetTimerManager().SetTimer(ShakeTimer, this, &ThisClass::FinishShaking, 0.5f, false);
		break;
	}
}

void ACameraManager::StopShaking()
{
	bIsApplyLoopShake = false;
	
	StopAllCameraShakes(true);
}

void ACameraManager::FinishShaking()
{
	bIsApplyLoopShake = false;
}
