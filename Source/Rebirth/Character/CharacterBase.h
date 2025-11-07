#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class USpotLightComponent;
class UPhysicsConstraintComponent;
class UInteractionComponent;
class ALantern;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class REBIRTH_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void DoMove(const FInputActionValue& Value);
	void DoInteract(const FInputActionValue& Value);

protected:
	/** 3인칭을 위한 카메라 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	/** 3인칭 카메라를 캐릭터 뒤에 위치시키기 위한 스프링암 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** 상호작용 액터 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;
		
	/** 이동 입력 액션입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> MoveAction;

	/** 상호작용 액션입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
	
	/** 랜턴 손잡이에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> LanternKnob;

	/** 랜턴에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> RootLantern;

	/** 랜턴에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> TargetLantern;

	/** 랜턴에 대한 물리엔진 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UPhysicsConstraintComponent> LanternJoint;
	
	/** 조명(SpotLight) 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category="컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLight;
	
};
