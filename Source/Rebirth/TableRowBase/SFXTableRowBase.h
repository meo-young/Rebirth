#pragma once

#include "CoreMinimal.h"
#include "SFXTableRowBase.generated.h"

enum class ESFX : uint8;
class USoundCue;

USTRUCT(BlueprintType)
struct FSFXTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ESFX SFX;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundCue> Sound;

};