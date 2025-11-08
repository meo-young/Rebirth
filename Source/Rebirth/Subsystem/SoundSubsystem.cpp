#include "SoundSubsystem.h"

#include "Rebirth.h"
#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Sound/SoundCue.h"
#include "TableRowBase/BGMTableRowBase.h"
#include "TableRowBase/SFXTableRowBase.h"
#include "Enum/EBGM.h"
#include "Enum/ESFX.h"
#include "Kismet/GameplayStatics.h"

USoundSubsystem::USoundSubsystem()
{
	// SFX 데이터테이블을 로드합니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> SFXDataTableObj(TEXT("/Game/_Rebirth/DataTable/DT_SFX.DT_SFX"));
	if (SFXDataTableObj.Succeeded())
	{
		SFXDataTable = SFXDataTableObj.Object;
		LOG(TEXT("SFX 데이터테이블 로드 성공"));
	}

	// BGM 데이터테이블을 로드합니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> BGMDataTableObj(TEXT("/Game/_Rebirth/DataTable/DT_BGM.DT_BGM"));
	if (BGMDataTableObj.Succeeded())
	{
		BGMDataTable = BGMDataTableObj.Object;
		LOG(TEXT("BGM 데이터테이블 로드 성공"));
	}
}

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 오디오 컴포넌트를 등록합니다.
	RegisterSFXComponent();
	RegisterBGMComponent();

	// 사운드 데이터테이블의 모든 행을 가져옵니다.
	SFXDataTable->GetAllRows("SFX", SFXTableRows);
	BGMDataTable->GetAllRows("BGM", BGMTableRows);
}

void USoundSubsystem::Deinitialize()
{
	SFXQueues.Empty();

	Super::Deinitialize();
}

void USoundSubsystem::PlaySFXInLocation(ESFX SFXType, const FVector& Location)
{
    const int32 Index = static_cast<int32>(SFXType);

    // 유효한 SFXType인지 확인합니다.
    if (!SFXTableRows.IsValidIndex(Index)) return;

    // 나중에 DataTable이 바뀌어도 안전하도록 SoftPtr만 캐싱
    const TSoftObjectPtr<USoundCue> SoundToLoad = SFXTableRows[Index]->Sound;
    if (SoundToLoad.IsNull()) return;

    // Subsystem이 사라졌을 때를 대비해서 Weak 포인터 사용
    TWeakObjectPtr<USoundSubsystem> WeakThis(this);
    const FVector TargetLocation = Location;

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        SoundToLoad.ToSoftObjectPath(),
        [WeakThis, SoundToLoad, TargetLocation]()
        {
            if (!WeakThis.IsValid()) return;

            USoundSubsystem* StrongThis = WeakThis.Get();
            if (!StrongThis) return;

            USoundCue* LoadedSound = SoundToLoad.Get();
            if (!LoadedSound) return;

            if (UWorld* World = StrongThis->GetWorld())
            {
                // 3D 위치 기반 SFX 재생
                UGameplayStatics::PlaySoundAtLocation(World, LoadedSound, TargetLocation);
            }
        }
    );
}

void USoundSubsystem::PlaySFX2D(ESFX SFXType)
{
    const int32 Index = static_cast<int32>(SFXType);
    if (!SFXTableRows.IsValidIndex(Index)) return;

    const TSoftObjectPtr<USoundCue> SoundToLoad = SFXTableRows[Index]->Sound;
    if (SoundToLoad.IsNull()) return;

    TWeakObjectPtr<USoundSubsystem> WeakThis(this);

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        SoundToLoad.ToSoftObjectPath(),
        [WeakThis, SoundToLoad]()
        {
            if (!WeakThis.IsValid()) return;

            USoundSubsystem* StrongThis = WeakThis.Get();
            if (!StrongThis) return;

            USoundCue* LoadedSound = SoundToLoad.Get();
            if (!LoadedSound) return;

            if (UWorld* World = StrongThis->GetWorld())
            {
                // 화면 전체에 들리는 2D 사운드 재생
                UGameplayStatics::PlaySound2D(World, LoadedSound);
            }
        }
    );
}

void USoundSubsystem::PlayBGM(EBGM BGMType)
{
	// 유효한 BGMType인지 확인합니다.
	if (!BGMAudioComponent || !BGMTableRows.IsValidIndex(static_cast<int32>(BGMType))) return;

	// 기존 페이드 아웃 중이면 중단
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(BGMFadeTimerHandle);
	}
	bIsBGMFading = false;

	// 볼륨 원복
	BGMAudioComponent->SetVolumeMultiplier(1.0f);

	// 비동기 로드를 사용하여 사운드를 로드하고 재생합니다.
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		BGMTableRows[static_cast<int32>(BGMType)]->Sound.ToSoftObjectPath(),
		[this, BGMType]()
		{
			USoundCue* LoadedSound = BGMTableRows[static_cast<int32>(BGMType)]->Sound.Get();
			if (!LoadedSound || !BGMAudioComponent) return;

			if (BGMAudioComponent->IsPlaying())
			{
				BGMAudioComponent->Stop();
			}

			BGMAudioComponent->SetSound(LoadedSound);
			BGMAudioComponent->Play();
		}
	);
}


void USoundSubsystem::StopBGM()
{
	if (!BGMAudioComponent) return;
	if (!BGMAudioComponent->IsPlaying() && !bIsBGMFading) return;

	if (UWorld* World = GetWorld())
	{
		// 페이드 초기값 세팅
		BGMStartVolume  = BGMAudioComponent->VolumeMultiplier;
		BGMFadeElapsed  = 0.0f;
		bIsBGMFading    = true;

		// 이미 돌고 있던 페이드 타이머 있으면 초기화
		World->GetTimerManager().ClearTimer(BGMFadeTimerHandle);

		// 0.01 ~ 0.02 초 간격으로 업데이트 (대략 60fps)
		World->GetTimerManager().SetTimer(
			BGMFadeTimerHandle,
			this,
			&USoundSubsystem::UpdateBGMFadeOut,
			0.016f,
			true
		);
	}
}

void USoundSubsystem::PauseBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->SetPaused(true);
	}
}

void USoundSubsystem::ResumeBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->bIsPaused)
	{
		BGMAudioComponent->SetPaused(false);
	}
}

void USoundSubsystem::OnSFXFinished(UAudioComponent* AudioComponent)
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		SFXQueues.Enqueue(AudioComponent);
	}
}

UAudioComponent* USoundSubsystem::GetPooledAudioComponent()
{
	UAudioComponent* AC = nullptr;

	// 큐에서 사용 가능한 오디오 컴포넌트를 가져옵니다.
	// 없다면 새로 생성합니다.
	if (!SFXQueues.Dequeue(AC))
	{
		UWorld* CachedWorld = GetWorld();
		AC = NewObject<UAudioComponent>(CachedWorld);
		AC->bAutoActivate = false;
		AC->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Comp) { OnSFXFinished(Comp); });
		AC->RegisterComponentWithWorld(CachedWorld);
	}

	return AC;
}

void USoundSubsystem::RegisterSFXComponent()
{
	// 오디오 컴포넌트 풀을 초기화합니다.
	SFXQueues.Empty();

	// 월드를 캐싱합니다.
	UWorld* CachedWorld = GetWorld();

	// 미리 생성된 오디오 컴포넌트를 큐에 추가합니다.
	for (int8 i = 0; i < 30; ++i)
	{
		UAudioComponent* AC = NewObject<UAudioComponent>(CachedWorld);
		AC->bAutoActivate = false;
		AC->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Comp) { OnSFXFinished(Comp); });
		AC->RegisterComponentWithWorld(CachedWorld);
		SFXQueues.Enqueue(AC);
	}
}

void USoundSubsystem::RegisterBGMComponent()
{
	// 월드를 캐싱합니다.
	UWorld* CachedWorld = GetWorld();

	// BGM 오디오 컴포넌트를 생성합니다.
	BGMAudioComponent = NewObject<UAudioComponent>(CachedWorld);
	BGMAudioComponent->bAutoActivate = false;
	BGMAudioComponent->RegisterComponentWithWorld(CachedWorld);
}

void USoundSubsystem::UpdateBGMFadeOut()
{
	if (!BGMAudioComponent)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(BGMFadeTimerHandle);
		}
		bIsBGMFading = false;
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		bIsBGMFading = false;
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();
	BGMFadeElapsed += DeltaTime;

	const float T = FMath::Clamp(BGMFadeElapsed / BGMFadeDuration, 0.0f, 1.0f);
	const float NewVolume = FMath::Lerp(BGMStartVolume, 0.0f, T);

	BGMAudioComponent->SetVolumeMultiplier(NewVolume);

	// 페이드 완료 시
	if (T >= 1.0f || NewVolume <= KINDA_SMALL_NUMBER)
	{
		BGMAudioComponent->SetVolumeMultiplier(0.0f);
		if (BGMAudioComponent->IsPlaying())
		{
			BGMAudioComponent->Stop();
		}

		World->GetTimerManager().ClearTimer(BGMFadeTimerHandle);
		bIsBGMFading = false;
	}
}
