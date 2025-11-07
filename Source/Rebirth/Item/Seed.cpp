#include "Item/Seed.h"

ASeed::ASeed()
{
	GuideText = "씨앗을 수집할 수 있습니다.";
}

void ASeed::Interact_Implementation()
{
	Super::Interact_Implementation();
}

void ASeed::ShowInteractGuide_Implementation()
{
	Super::ShowInteractGuide_Implementation();
}
