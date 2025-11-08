#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoundSubsystem.generated.h"

struct FBGMTableRowBase;
struct FSFXTableRowBase;
enum class ESFX : uint8;
enum class EBGM : uint8;

UCLASS()
class REBIRTH_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USoundSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	/** 특정 위치에서 SFX를 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void PlaySFXInLocation(ESFX SFX, const FVector& Location);

	/** 위치와 관계없이 제자리에서 SFX를 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void PlaySFX2D(ESFX SFX);

	/** BGM을 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void PlayBGM(EBGM BGM);

	/** BGM을 중지하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void StopBGM();

	/** BGM을 일시정지하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void PauseBGM();

	/** BGM을 재개하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void ResumeBGM();

private:
	/** SFX 재생이 끝났을 때 호출되는 콜백 함수입니다. */
	UFUNCTION()
	void OnSFXFinished(UAudioComponent* AudioComponent);

	/** 오디오 컴포넌트 풀에서 사용 가능한 컴포넌트를 가져오는 함수입니다. */
	UAudioComponent* GetPooledAudioComponent();

	void RegisterSFXComponent();
	void RegisterBGMComponent();

private:
	/** SFX에 대한 데이터 테이블입니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> SFXDataTable = nullptr;

	/** BGM에 대한 데이터 테이블입니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> BGMDataTable = nullptr;

	/** BGM 오디오 컴포넌트입니다. */
	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMAudioComponent;

	/** BGM 데이터테이블의 모든 행을 저장하는 배열입니다. */
	TArray<FBGMTableRowBase*> BGMTableRows;
	
	/** SFX 오디오 컴포넌트 풀입니다. */
	TQueue<UAudioComponent*> SFXQueues;

	/** SFX 데이터테이블의 모든 행을 저장하는 배열입니다. */
	TArray<FSFXTableRowBase*> SFXTableRows;

private:
	// BGM 페이드 아웃용
	FTimerHandle BGMFadeTimerHandle;
	float BGMFadeDuration = 2.0f;   // 2초 동안
	float BGMFadeElapsed  = 0.0f;
	float BGMStartVolume  = 1.0f;
	bool  bIsBGMFading    = false;

	void UpdateBGMFadeOut();
	
};