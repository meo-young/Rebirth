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
	EAIState CurrentState;
	uint8 bIsDamaged : 1 = false;
	void ApplyDamagedState();
	void ExitDamagedState();

protected:
	/** Monster Mesh입니다. */
	UPROPERTY(EditDefaultsOnly, Category="변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> SubMesh1;

	/** Monster Mesh입니다. */
	UPROPERTY(EditDefaultsOnly, Category="변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> SubMesh2;


};
