#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void DoMove(const FInputActionValue& Value);

protected:
	/** 3인칭을 위한 카메라 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	/** 3인칭 카메라를 캐릭터 뒤에 위치시키기 위한 스프링암 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/** 이동 입력 액션입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> MoveAction;
	
};
