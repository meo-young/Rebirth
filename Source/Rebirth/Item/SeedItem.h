#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "SeedItem.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

UCLASS(Blueprintable)
class REBIRTH_API ASeedItem : public AItemBase
{
	GENERATED_BODY()

public:
	ASeedItem();

public:
	/** Interactable 인터페이스의 Interact 함수 구현 */
	virtual void Interact_Implementation() override;
	virtual void ShowInteractGuide_Implementation() override;

private:
	void ShowDialogue();
	void ShowFakeDialogue();
	void HideFakeDialogue();
	void EndDialogue();
	void PlayLevelSequence();

protected:
	UPROPERTY(EditAnywhere, Category = "변수|대사")
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

	UPROPERTY(EditAnywhere, Category = "변수|상태")
	uint8 bIsReal : 1 = false;

	UPROPERTY(EditAnywhere, Category = "변수|상태")
	int32 SeedIndex = 0;
	
	
private:
	FTimerHandle FadeTimer;
	
};
