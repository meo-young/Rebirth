#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuideWidget.generated.h"

class UTextBlock;

UCLASS()
class REBIRTH_API UGuideWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	/** Guide를 Fade 연출로 활성화하는 함수입니다 */
	UFUNCTION(BlueprintCallable)
	void ShowGuide(const FString& InText);

	/** Guide를 Fade 연출로 비활성화하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void HideGuide();

private:
	/** Guide가 표시되는 텍스트 박스입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Guide_Text;

	/** Guide Text가 활성화 되고 있는지에 대한 상태 변수입니다. */
	uint8 bIsActive : 1 = false;

private:
	FTimerHandle FadeTimerHandle;
	float FadeDuration = 1.5f;
	float FadeElapsed  = 0.0f;
	float FadeStart    = 0.0f;
	float FadeTarget   = 1.0f;   // 0 or 1
	bool  bFading      = false;

	void UpdateFade();
	void EnsureFadeTimerRunning();

	
};
