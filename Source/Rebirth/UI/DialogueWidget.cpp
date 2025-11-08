#include "UI/DialogueWidget.h"
#include "Components/Image.h"
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

	
	if (Dialogue_Image)
	{
		Dialogue_Image->SetRenderOpacity(0.0f);
	}
}

void UDialogueWidget::ShowDialogue(const FString& InText, const FLinearColor& InColor, int32 InImageType)
{
	if (!Dialogue_Text) return;
	
	// 텍스트, 색상 갱신
	Dialogue_Text->SetText(FText::FromString(InText));
	Dialogue_Text->SetColorAndOpacity(InColor);

	// ⬇ 여기서 이미지도 Stage 타입에 맞게 변경
	if (Dialogue_Image)
	{
		if (UTexture2D* const* FoundTex = StageImages.Find(InImageType))
		{
			Dialogue_Image->SetBrushFromTexture(*FoundTex);
		}
		// 텍스트와 함께 Fade 되도록, 시작 시 현재 opacity 가져오기만 하고
		// UpdateFade에서 같이 조절
	}

	const float Current = Dialogue_Text->GetRenderOpacity();

	// 이미 목표가 1.0이라면: 리셋 금지(그대로 진행/유지)
	if (FMath::IsNearlyEqual(FadeTarget, 1.0f))
	{
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
	if (!Dialogue_Text && !Dialogue_Image)
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

	if (Dialogue_Text)
	{
		Dialogue_Text->SetRenderOpacity(NewOpacity);
	}

	if (Dialogue_Image)
	{
		Dialogue_Image->SetRenderOpacity(NewOpacity); // ⬅ 이미지도 같이 페이드
	}

	const bool bFinished = (T >= 1.0f) || FMath::IsNearlyEqual(NewOpacity, FadeTarget, 0.001f);
	if (bFinished)
	{
		if (Dialogue_Text)
		{
			Dialogue_Text->SetRenderOpacity(FadeTarget);
		}
		if (Dialogue_Image)
		{
			Dialogue_Image->SetRenderOpacity(FadeTarget); // ⬅ 최종값 고정
		}

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(FadeTimerHandle);
		}
		bFading = false;

		if (FMath::IsNearlyZero(FadeTarget))
		{
			if (Dialogue_Text)
			{
				Dialogue_Text->SetText(FText::GetEmpty());
			}
			// 필요하면 이미지도 비워주기
			// if (Dialogue_Image)
			// {
			//     Dialogue_Image->SetBrushFromTexture(nullptr);
			// }
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
