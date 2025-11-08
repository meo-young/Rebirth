#include "UI/FadeWidget.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 시작 시 완전 투명으로 설정
	if (FadeImage)
	{
		FadeImage->SetRenderOpacity(0.f);
		FadeImage->SetColorAndOpacity(FLinearColor::White);
	}
}

void UFadeWidget::FadeIn(const FLinearColor& InColor)
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

	bIsFadingIn = true;
	CurrentAlpha = 0.f;
	TargetColor = InColor;

	FadeImage->SetColorAndOpacity(TargetColor); // 색상 적용

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UFadeWidget::UpdateFade,
		0.01f,
		true
	);
}

void UFadeWidget::FadeOut(const FLinearColor& InColor)
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

	bIsFadingIn = false;
	CurrentAlpha = 1.f;
	TargetColor = InColor;

	FadeImage->SetColorAndOpacity(TargetColor); // 색상 적용

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UFadeWidget::UpdateFade,
		0.01f,
		true
	);
}

void UFadeWidget::UpdateFade()
{
	const float DeltaAlpha = 0.01f / FadeDuration; // 2초 동안 0~1 변화

	if (bIsFadingIn)
	{
		CurrentAlpha = FMath::Clamp(CurrentAlpha + DeltaAlpha, 0.f, 1.f);
	}
	else
	{
		CurrentAlpha = FMath::Clamp(CurrentAlpha - DeltaAlpha, 0.f, 1.f);
	}

	FadeImage->SetRenderOpacity(CurrentAlpha);

	// 종료 조건
	if ((bIsFadingIn && CurrentAlpha >= 1.f) ||
		(!bIsFadingIn && CurrentAlpha <= 0.f))
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}
}

void UFadeWidget::SetInstantVisible(const FLinearColor& InColor)
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	CurrentAlpha = 1.f;
	FadeImage->SetColorAndOpacity(InColor);
	FadeImage->SetRenderOpacity(1.f);
}

void UFadeWidget::SetInstantInvisible(const FLinearColor& InColor)
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	CurrentAlpha = 0.f;
	FadeImage->SetRenderOpacity(0.f);
}
