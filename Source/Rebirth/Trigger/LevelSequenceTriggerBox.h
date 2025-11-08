#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelSequenceTriggerBox.generated.h"

class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;

UCLASS()
class REBIRTH_API ALevelSequenceTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	/** 오버랩 시작 시 호출되는 함수 */
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	/** 재생할 Level Sequence 에셋 */
	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	TObjectPtr<ULevelSequence> LevelSequenceAsset;

	/** 내부에서 생성해서 재사용할 플레이어 */
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	/** 플레이어 생성 시 함께 만들어지는 LevelSequenceActor */
	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor = nullptr;
};
