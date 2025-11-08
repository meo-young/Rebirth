#include "UI/FadeWidget.h"
#include "Components/Image.h"

void UFadeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 시작 시 완전 투명으로 설정
	if (FadeImage)
	{
		FadeImage->SetRenderOpacity(0.f);
	}
}

void UFadeWidget::FadeIn()
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	bIsFadingIn = true;
	CurrentAlpha = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UFadeWidget::UpdateFade,
		0.01f,     // 0.01초마다 업데이트 (부드럽게)
		true
	);
}

void UFadeWidget::FadeOut()
{
	if (!FadeImage) return;

	GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	bIsFadingIn = false;
	CurrentAlpha = 1.f;

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
