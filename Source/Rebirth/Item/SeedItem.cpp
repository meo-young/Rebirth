#include "Item/SeedItem.h"

#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Character/CharacterBase.h"
#include "Character/PlayerControllerBase.h"
#include "Enum/ESFX.h"
#include "Kismet/GameplayStatics.h"
#include "Library/SoundLibrary.h"
#include "UI/DialogueWidget.h"
#include "UI/FadeWidget.h"
#include "UI/GuideWidget.h"

ASeedItem::ASeedItem()
{
	GuideText = "씨앗을 수집할 수 있습니다.";
}

void ASeedItem::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (bIsReal)
	{
		GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::ShowDialogue, 1.5f, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::ShowFakeDialogue, 1.5f, false);
	}
}

void ASeedItem::ShowInteractGuide_Implementation()
{
	Super::ShowInteractGuide_Implementation();
}

void ASeedItem::ShowDialogue()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		USoundLibrary::PlaySFX2D(PlayerController, ESFX::Seed);
		Cast<ACharacterBase>(PlayerController->GetPawn())->bIsCanMove = false;
		PlayerController->DialogueWidgetInstance->ShowDialogue(Dialogue, FLinearColor::Black, SeedIndex);
		PlayerController->FadeWidgetInstance->FadeIn(FLinearColor::White);
		GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::EndDialogue, 4.0f, false);
	}
}

void ASeedItem::ShowFakeDialogue()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		USoundLibrary::PlaySFX2D(PlayerController, ESFX::Seed);
		Cast<ACharacterBase>(PlayerController->GetPawn())->bIsCanMove = false;
		PlayerController->DialogueWidgetInstance->ShowDialogue(Dialogue, FLinearColor::White);
		GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::HideFakeDialogue, 1.5f, false);
	}
}

void ASeedItem::HideFakeDialogue()
{
	SeedMesh->SetVisibility(false);
	
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		Cast<ACharacterBase>(PlayerController->GetPawn())->bIsCanMove = true;
		PlayerController->DialogueWidgetInstance->HideDialogue ();
	}
}

void ASeedItem::EndDialogue()
{
	SeedMesh->SetVisibility(false);
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->DialogueWidgetInstance->HideDialogue ();
		PlayerController->FadeWidgetInstance->FadeOut(FLinearColor::White);
		GetWorldTimerManager().SetTimer(FadeTimer, this, &ThisClass::PlayLevelSequence, 2.0f, false);
	}
	
}

void ASeedItem::PlayLevelSequence()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		Cast<ACharacterBase>(PlayerController->GetPawn())->bIsCanMove = true;
		
	}
	
	// 아직 플레이어가 없으면 생성
	if (!IsValid(LevelSequencePlayer))
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;   // 우리가 직접 Play() 호출할 거라 수동 재생

		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			LevelSequenceAsset,
			Settings,
			LevelSequenceActor
		);
	}

	// 플레이어가 유효하면 재생
	if (IsValid(LevelSequencePlayer))
	{
		LevelSequencePlayer->Play();
	}
}
