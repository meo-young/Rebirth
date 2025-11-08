#include "LevelSequenceTriggerBox.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

void ALevelSequenceTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 델리게이트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &ALevelSequenceTriggerBox::OnOverlapBegin);
}

void ALevelSequenceTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// 유효한 OtherActor인지 확인
	if (!IsValid(OtherActor) || OtherActor == this)
	{
		return;
	}

	// 플레이어만 트리거하도록 제한
	if (!OtherActor->IsA(ACharacterBase::StaticClass())) return;

	// 시퀀스 에셋이 없으면 종료
	if (!IsValid(LevelSequenceAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelSequenceAsset is not set on %s"), *GetName());
		return;
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
