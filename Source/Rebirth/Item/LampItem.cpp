#include "Item/LampItem.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Character/CharacterBase.h"
#include "Character/PlayerControllerBase.h"
#include "Enum/ESFX.h"
#include "Kismet/GameplayStatics.h"
#include "Library/SoundLibrary.h"

ALampItem::ALampItem()
{
	GuideText = "[F] 키를 누르면 개울을 넘어갈 길을 만들 수 있습니다.";
}

void ALampItem::Interact_Implementation()
{
	Super::Interact_Implementation();

	// 2️⃣ 1.5초 후 ShowDialogue 실행
	GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::PlayLevelSequence, 1.5f, false);
}

void ALampItem::ShowInteractGuide_Implementation()
{
	Super::ShowInteractGuide_Implementation();
}


void ALampItem::PlayLevelSequence()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		Cast<ACharacterBase>(PlayerController->GetPawn())->bIsCanMove = true;
		
	}
	
	// 아직 플레이어가 없으면 생성
	if (!IsValid(LevelSequencePlayer))
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;   // 우리가 직접 Play() 호출할 거라 수동 재생

		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			LevelSequenceAsset,
			Settings,
			LevelSequenceActor
		);
	}

	// 플레이어가 유효하면 재생
	if (IsValid(LevelSequencePlayer))
	{
		LevelSequencePlayer->Play();
	}
}
