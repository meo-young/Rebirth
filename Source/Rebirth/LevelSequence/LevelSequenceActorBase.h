#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "LevelSequenceActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceEnded);

class ULevelSequence;

UCLASS()
class REBIRTH_API ALevelSequenceActorBase : public ALevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
public:
	/** LevelSequence가 시작될 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnSequenceStarted OnSequenceStartedDelegate;

	/** LevelSequence가 종료될 때 호출되는 델리게이트 */
	UPROPERTY(BlueprintAssignable)
	FOnSequenceEnded OnSequenceEndedDelegate;
	
public:
	/** 등록된 LevelSequence를 재생하는 함수 */
	UFUNCTION(BlueprintCallable)
	void PlayLevelSequence();

	/** 등록된 LevelSequence를 중단하는 함수 */
	UFUNCTION(BlueprintCallable)
	void StopLevelSequence();
	
	/** LevelSequence의 종료 델리게이트에 바인딩할 함수 */
	UFUNCTION(BlueprintCallable)
	void OnSequenceEnded();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> LevelSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer = nullptr;
	
};
