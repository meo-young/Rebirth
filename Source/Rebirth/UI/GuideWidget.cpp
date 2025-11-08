#include "UI/GuideWidget.h"
#include "Rebirth.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UGuideWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (Guide_Text)
	{
		Guide_Text->SetText(FText::GetEmpty());
		Guide_Text->SetRenderOpacity(0.0f);
	}
}

void UGuideWidget::ShowGuide(const FString& InText, const FLinearColor& InColor)
{
    if (!Guide_Text) return;
    
    LOG(TEXT("호출"));

    // 텍스트, 색상 갱신
    Guide_Text->SetText(FText::FromString(InText));
    Guide_Text->SetColorAndOpacity(InColor); // 여기서 색 변경

    const float Current = Guide_Text->GetRenderOpacity();

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

void UGuideWidget::HideGuide()
{
    if (!Guide_Text) return;
    
    const float Current = Guide_Text->GetRenderOpacity();

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

void UGuideWidget::EnsureFadeTimerRunning()
{
    if (UWorld* World = GetWorld())
    {
        FTimerManager& TM = World->GetTimerManager();
        // 이미 돌고 있으면 건드리지 않음(리셋 방지)
        if (!TM.IsTimerActive(FadeTimerHandle))
        {
            TM.SetTimer(FadeTimerHandle, this, &UGuideWidget::UpdateFade, 0.016f, true);
        }
    }
}

void UGuideWidget::UpdateFade()
{
    if (!Guide_Text)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(FadeTimerHandle);
        }
        bFading = false;
        return;
    }

    const float Delta = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.016f;
    FadeElapsed += Delta;

    const float T = FMath::Clamp(FadeElapsed / FadeDuration, 0.0f, 1.0f);
    const float NewOpacity = FMath::Lerp(FadeStart, FadeTarget, T);
    Guide_Text->SetRenderOpacity(NewOpacity);

    const bool bFinished = (T >= 1.0f) || FMath::IsNearlyEqual(NewOpacity, FadeTarget, 0.001f);
    if (bFinished)
    {
        Guide_Text->SetRenderOpacity(FadeTarget);

        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(FadeTimerHandle);
        }
        bFading = false;

        if (FMath::IsNearlyZero(FadeTarget))
        {
            Guide_Text->SetText(FText::GetEmpty());
        }
    }
}
