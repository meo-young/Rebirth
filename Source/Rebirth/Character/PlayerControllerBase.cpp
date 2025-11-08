#include "Character/PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Rebirth.h"
#include "UI/DialogueWidget.h"
#include "UI/FadeWidget.h"
#include "UI/GuideWidget.h"

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 로컬 플레이어의 Enhanced Input 서브시스템을 가져와서 매핑 컨텍스트를 추가합니다.
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(MappingContext, 0);
	}
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(FadeWidgetClass))
	{
		FadeWidgetInstance = CreateWidget<UFadeWidget>(GetWorld(), FadeWidgetClass);
		if (IsValid(FadeWidgetInstance))
		{
			if (!FadeWidgetInstance->IsInViewport())
			{
				LOG(TEXT("FadeWidget 인스턴스 생성 성공"));
				FadeWidgetInstance->AddToViewport(5);
			}
		}
	}

	if (IsValid(GuideWidgetClass))
	{
		GuideWidgetInstance = CreateWidget<UGuideWidget>(GetWorld(), GuideWidgetClass);
		if (IsValid(GuideWidgetInstance))
		{
			if (!GuideWidgetInstance->IsInViewport())
			{
				LOG(TEXT("GuideWidget 인스턴스 생성 성공"));
				GuideWidgetInstance->AddToViewport(10);
			}
		}
	}

	if (IsValid(DialogueWidgetClass))
	{
		DialogueWidgetInstance = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);
		if (IsValid(DialogueWidgetInstance))
		{
			if (!DialogueWidgetInstance->IsInViewport())
			{
				LOG(TEXT("DialogueWidgetInstance 인스턴스 생성 성공"));
				DialogueWidgetInstance->AddToViewport(20);
			}
		}
	}
}
