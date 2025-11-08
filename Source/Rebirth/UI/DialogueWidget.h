#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UTextBlock;

UCLASS()
class REBIRTH_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	/** Dialogue를 Fade 연출로 활성화하는 함수입니다 */
	UFUNCTION(BlueprintCallable)
	void ShowDialogue(const FString& InText);

	/** Dialogue를 Fade 연출로 비활성화하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void HideDialogue();
	
private:
	/** Dialogue가 표시되는 텍스트 박스입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Dialogue_Text;

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
