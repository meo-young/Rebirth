// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define LOG(Format, ...) UE_LOG(AMeoyoung, Warning, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__));

DECLARE_LOG_CATEGORY_EXTERN(AMeoyoung, Warning, All);