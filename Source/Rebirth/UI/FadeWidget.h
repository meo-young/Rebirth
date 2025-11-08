#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"

class UImage;

UCLASS()
class REBIRTH_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Fade In 연출을 출력합니다. */
	void FadeIn();

	/** Fade Out 연출을 출력합니다. */
	void FadeOut();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;

	FTimerHandle FadeTimerHandle;
	float CurrentAlpha = 0.f;
	float FadeDuration = 2.f; // 2초 동안
	bool bIsFadingIn = false;

	void UpdateFade();
};
