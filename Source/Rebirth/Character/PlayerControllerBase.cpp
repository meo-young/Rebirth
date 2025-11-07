#include "Character/PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 로컬 플레이어의 Enhanced Input 서브시스템을 가져와서 매핑 컨텍스트를 추가합니다.
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(MappingContext, 0);
	}
}