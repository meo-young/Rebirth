#pragma once

#include "CoreMinimal.h"
#include "BGMTableRowBase.generated.h"

enum class EBGM : uint8;
class USoundCue;

USTRUCT(BlueprintType)
struct FBGMTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EBGM BGM;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundCue> Sound;
};