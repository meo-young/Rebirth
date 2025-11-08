#include "UI/DialogueWidget.h"

#include "Rebirth.h"
#include "Components/TextBlock.h"

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Dialogue_Text)
	{
		Dialogue_Text->SetText(FText::GetEmpty());
		Dialogue_Text->SetRenderOpacity(0.0f);
	}
}

void UDialogueWidget::ShowDialogue(const FString& InText, const FLinearColor& InColor)
{
	if (!Dialogue_Text) return;
	
	// 텍스트, 색상 갱신
	Dialogue_Text->SetText(FText::FromString(InText));
	Dialogue_Text->SetColorAndOpacity(InColor); // 여기서 색 변경
	
	const float Current = Dialogue_Text->GetRenderOpacity();
	

	// 이미 목표가 1.0이라면: 리셋 금지(그대로 진행/유지)
	if (FMath::IsNearlyEqual(FadeTarget, 1.0f))
	{
		// 타이머가 꺼져 있는 경우만 켠다(완료 상태일 수도 있으니)
		EnsureFadeTimerRunning();
		return;
	}

	// 방향 전환(0 -> 1)
	FadeStart   = Current; // 현재 지점에서
	FadeTarget  = 1.0f;    // 목표는 1
	FadeElapsed = 0.0f;
	bFading     = true;

	EnsureFadeTimerRunning();
}

void UDialogueWidget::HideDialogue()
{
	if (!Dialogue_Text) return;
	
	const float Current = Dialogue_Text->GetRenderOpacity();

	// 이미 목표가 0.0이라면: 리셋 금지(그대로 진행/유지)
	if (FMath::IsNearlyZero(FadeTarget))
	{
		EnsureFadeTimerRunning();
		return;
	}

	// 방향 전환(1 -> 0)
	FadeStart   = Current;        // 현재 지점에서
	FadeTarget  = 0.0f;           // 목표는 0
	FadeElapsed = 0.0f;
	bFading     = true;

	EnsureFadeTimerRunning();
}

void UDialogueWidget::UpdateFade()
{
	if (!Dialogue_Text)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(FadeTimerHandle);
		}
		bFading = false;
		return;
	}

	LOG(TEXT("업데이트중"));

	const float Delta = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.016f;
	FadeElapsed += Delta;

	const float T = FMath::Clamp(FadeElapsed / FadeDuration, 0.0f, 1.0f);
	const float NewOpacity = FMath::Lerp(FadeStart, FadeTarget, T);
	Dialogue_Text->SetRenderOpacity(NewOpacity);

	const bool bFinished = (T >= 1.0f) || FMath::IsNearlyEqual(NewOpacity, FadeTarget, 0.001f);
	if (bFinished)
	{
		Dialogue_Text->SetRenderOpacity(FadeTarget);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(FadeTimerHandle);
		}
		bFading = false;

		if (FMath::IsNearlyZero(FadeTarget))
		{
			Dialogue_Text->SetText(FText::GetEmpty());
		}
	}
}

void UDialogueWidget::EnsureFadeTimerRunning()
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TM = World->GetTimerManager();
		// 이미 돌고 있으면 건드리지 않음(리셋 방지)
		if (!TM.IsTimerActive(FadeTimerHandle))
		{
			TM.SetTimer(FadeTimerHandle, this, &UDialogueWidget::UpdateFade, 0.016f, true);
		}
	}
}
