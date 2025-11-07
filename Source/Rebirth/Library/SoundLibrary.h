#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoundLibrary.generated.h"

class USoundSubsystem;
enum class EBGM : uint8;
enum class ESFX : uint8;

UCLASS()
class REBIRTH_API USoundLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 특정 위치에서 SFX를 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void PlaySFXInLocation(const UObject* WorldContextObject, ESFX SFX, const FVector& Location);

	/** 위치와 관계없이 제자리에서 SFX를 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void PlaySFX2D(const UObject* WorldContextObject, ESFX SFX);

	/** BGM을 재생하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void PlayBGM(const UObject* WorldContextObject, EBGM BGM);

	/** BGM을 중지하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void StopBGM(const UObject* WorldContextObject);

	/** BGM을 일시정지하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void PauseBGM(const UObject* WorldContextObject);

	/** BGM을 재개하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void ResumeBGM(const UObject* WorldContextObject);

private:
	/** 월드 컨텍스트 객체로부터 사운드 서브시스템을 가져오는 헬퍼 함수입니다. */
	static USoundSubsystem* GetSoundSubsystem(const UObject* WorldContextObject);
	
};