#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class AAICharacter;

namespace EDrawDebugTrace
{
	enum Type : int;
}

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

	/** Spot Light의 범위에 맞게 Cone Tracing을 실시합니다. */
	void SpotLightTracing();

	/** Cone Tracing을 실시하는 함수입니다. */
	UFUNCTION(BlueprintCallable, Category = "Collision", Meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Multi Cone Trace By Channel", AdvancedDisplay = "TraceColor, TraceHitColor, DrawTime", Keywords = "sweep"))
	bool ConeTraceMulti(const UObject* WorldContextObject, const FVector Start, const FRotator Direction, float ConeHeight, float ConeHalfAngle, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

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

	/** 회전 보간 속도입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|스탯")
	float RotationInterpSpeed = 8.0f;
	
private:
	/** 히트결과를 다른 로직에서 쓰고 싶으면 멤버로 보관 */
	FHitResult SpotTraceHit;

	/** 지난 프레임에 SpotLight에 포착됐던 AI들 */
	UPROPERTY()
	TSet<TWeakObjectPtr<AAICharacter>> PrevSpotlightHitSet;
	
};
