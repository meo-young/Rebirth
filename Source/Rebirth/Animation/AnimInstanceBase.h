#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

class UCharacterMovementComponent;
class ACharacterBase;

UCLASS()
class REBIRTH_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	/** 이 애니메이션 인스턴스가 소유하고 있는 캐릭터입니다. */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacterBase> Player = nullptr;
	
protected:
	/** 캐릭터의 이동 속도에 대한 변수입니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수")
	float MoveSpeed;

	/** 캐릭터가 이동하고 있는지에 대한 상태 변수입니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수")
	uint8 bIsMoving : 1 = false;

private:
	/** 캐릭터의 이동 컴포넌트입니다. */
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;
	
};
