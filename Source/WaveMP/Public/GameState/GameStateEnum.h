// Copyright Michael Freeman
#pragma once

#include "CoreMinimal.h"
#include "GameStateEnum.generated.h"

UENUM(BlueprintType)
enum class EGameStatus : uint8
{
	Waiting UMETA(DisplayName = "Waiting"),
	Playing UMETA(DisplayName = "Playing"),
	EndGame UMETA(DisplayName = "EndGame")
};