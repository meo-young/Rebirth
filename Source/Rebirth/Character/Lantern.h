#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lantern.generated.h"

class USpotLightComponent;
class UPhysicsConstraintComponent;

UCLASS()
class REBIRTH_API ALantern : public AActor
{
	GENERATED_BODY()
	
public:	
	ALantern();
	virtual void BeginPlay() override;

public:
	/** 랜턴 손잡이에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> LanternKnob;

	/** 랜턴에 대한 메시입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Lantern;
	
protected:
	/** Root 씬 컴포넌트입니다. */
	UPROPERTY(VisibleDefaultsOnly, Category="컴포넌트")
	TObjectPtr<USceneComponent> Root;

	/** 랜턴에 대한 물리엔진 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UPhysicsConstraintComponent> LanternJoint;
	
	/** 조명(SpotLight) 컴포넌트입니다. */
	UPROPERTY(EditDefaultsOnly, Category="컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLight;
	
};
