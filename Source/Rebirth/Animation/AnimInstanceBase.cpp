#include "Animation/AnimInstanceBase.h"

#include "Rebirth.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Player = Cast<ACharacterBase>(TryGetPawnOwner());
	if (!Player)
	{
		LOG(TEXT("PlayerAnimInstance에서 소유한 캐릭터를 찾을 수 없습니다."));
		return;
	}

	MovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
	if (!MovementComponent)
	{
		LOG(TEXT("PlayerAnimInstance에서 캐릭터의 이동 컴포넌트를 찾을 수 없습니다."));
		return;
	}
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Player || !MovementComponent) return;

	// 캐릭터의 이동 속도를 계산합니다.
	MoveSpeed = MovementComponent->Velocity.Size();

	// 캐릭터의 이동속도가 0에 수렴하면, 이동변수 값을 false로 저장합니다.
	if (MoveSpeed <= KINDA_SMALL_NUMBER)
	{
		bIsMoving = false;
	}
	else
	{
		bIsMoving = true;
	}
}