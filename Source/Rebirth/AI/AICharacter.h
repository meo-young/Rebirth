#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UENUM()
enum class EAIState : uint8
{
	Idle = 0,
	Move = 1,
	Attack = 2,
	Damaged = 3
};

UCLASS()
class REBIRTH_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

public:
	void ApplyDamagedState();
	void ExitDamagedState();
	void PlayDamagedMontage();

public:
	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TObjectPtr<UAnimMontage> DamagedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TObjectPtr<UAnimMontage> SubDamagedMontage1;

	UPROPERTY(EditDefaultsOnly, Category = "변수|몽타주")
	TObjectPtr<UAnimMontage> SubDamagedMontage2;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|인자")
	float KnockbackThreshold = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|인자")
	float CameraShakeScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|인자")
	float KnockbackScale = 2500.0f;

protected:
	/** Monster Mesh입니다. */
	UPROPERTY(EditDefaultsOnly, Category="변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> SubMesh1;

	/** Monster Mesh입니다. */
	UPROPERTY(EditDefaultsOnly, Category="변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> SubMesh2;


};
