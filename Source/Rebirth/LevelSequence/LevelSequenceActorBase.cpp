#include "LevelSequence/LevelSequenceActorBase.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "Rebirth.h"

void ALevelSequenceActorBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 등록된 레벨 시퀀스를 추출한다.
	LevelSequence = GetSequence();
	if (!IsValid(LevelSequence))
	{
		LOG(TEXT("LevelSequence Is Not Valid !"));
		return;
	}

	if (LevelSequencePlayer) return;

	// 재생 전에 레벨 시퀀스 플레이어를 변수화 해야 종료 델리게이트에서 활용할 수 있다.
	ALevelSequenceActor* OutActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
}

void ALevelSequenceActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LevelSequencePlayer))
	{
		// 종료 델리게이트에 함수를 바인딩한다.
		LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnSequenceEnded);

		/*// ① 시퀀스 워밍업
		constexpr float WarmupTime = 0.01f; // 0.01초 정도 앞으로 보냄
		LevelSequencePlayer->SetPlayRate(0.f);        // 정지된 상태로
		LevelSequencePlayer->Play();                  // Evaluate를 강제로 한 번 돌리고
		LevelSequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(
			WarmupTime, EUpdatePositionMethod::Play
		));
		LevelSequencePlayer->Stop();                  // 다시 멈추고
		LevelSequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(
			0.f, EUpdatePositionMethod::Jump
		)); */
	}
}

void ALevelSequenceActorBase::PlayLevelSequence()
{
	LOG(TEXT("%s 를 재생합니다."), *GetName());
	if (OnSequenceStartedDelegate.IsBound())
	{
		LOG(TEXT("%s 시퀀스 시작 델리게이트 호출 시도합니다."), *GetName());
		OnSequenceStartedDelegate.Broadcast();
	}

	LevelSequencePlayer->Play();
}

void ALevelSequenceActorBase::StopLevelSequence()
{
	LOG(TEXT("%s 를 중단합니다."), *GetName());
	LevelSequencePlayer->Stop();
}

void ALevelSequenceActorBase::OnSequenceEnded()
{
	LOG(TEXT("%s 시퀀스 종료 델리게이트 호출 시도합니다."), *GetName());

	if (OnSequenceEndedDelegate.IsBound())
	{
		LOG(TEXT("%s 시퀀스 종료 델리게이트 호출합니다."), *GetName());
		OnSequenceEndedDelegate.Broadcast();
	}
}