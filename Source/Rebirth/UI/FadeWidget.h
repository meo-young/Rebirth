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
	UFUNCTION(BlueprintCallable)
	void FadeIn(const FLinearColor& InColor = FLinearColor::Black);

	/** Fade Out 연출을 출력합니다. */
	UFUNCTION(BlueprintCallable)
	void FadeOut(const FLinearColor& InColor = FLinearColor::Black);
	
	/** 즉시 완전 불투명(1.0f)으로 변경 */
	UFUNCTION(BlueprintCallable)
	void SetInstantVisible(const FLinearColor& InColor = FLinearColor::Black);

	/** 즉시 완전 투명(0.0f)으로 변경 */
	UFUNCTION(BlueprintCallable)
	void SetInstantInvisible(const FLinearColor& InColor = FLinearColor::Black);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;

	FTimerHandle FadeTimerHandle;
	FLinearColor TargetColor = FLinearColor::Black;
	float CurrentAlpha = 0.f;
	float FadeDuration = 2.f; // 2초 동안
	bool bIsFadingIn = false;

	void UpdateFade();
};
