#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Seed.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

UCLASS()
class REBIRTH_API ASeed : public AItemBase
{
	GENERATED_BODY()

public:
	ASeed();

public:
	/** Interactable 인터페이스의 Interact 함수 구현 */
	virtual void Interact_Implementation() override;
	virtual void ShowInteractGuide_Implementation() override;

private:
	void ShowDialogue();
	void EndDialogue();
	void PlayLevelSequence();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|대사")
	FString Dialogue;

	/** 재생할 Level Sequence 에셋 */
	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	TObjectPtr<ULevelSequence> LevelSequenceAsset;

	/** 내부에서 생성해서 재사용할 플레이어 */
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	/** 플레이어 생성 시 함께 만들어지는 LevelSequenceActor */
	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor = nullptr;
	
private:
	FTimerHandle FadeTimer;
	
};
