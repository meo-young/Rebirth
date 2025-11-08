#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CameraManager.generated.h"

UENUM()
enum class EShakeState : uint8
{
	Monster_Loop = 0,
	Monster_Damaged = 1
};

UCLASS()
class REBIRTH_API ACameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	void ApplyShake(const EShakeState ShakeState, const float Scale);
	void StopShaking();
	void FinishShaking();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|쉐이킹")
	TSubclassOf<UCameraShakeBase> MonsterLoopShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "변수|쉐이킹")
	TSubclassOf<UCameraShakeBase> MonsterDamagedShakeClass;

private:
	FTimerHandle ShakeTimer;
	uint8 bIsApplyLoopShake : 1 = false;
	
};
