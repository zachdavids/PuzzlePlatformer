// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PuzzlePlatformerGameMode.generated.h"

class UWinnerMenu;

UCLASS(minimalapi)
class APuzzlePlatformerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	APuzzlePlatformerGameMode();

	UFUNCTION(BlueprintCallable)
	void EndRound(FString WinnerName);

	virtual void ResetLevel() override;

private:
	FTimerHandle EndRoundTimerHandle;

	UWinnerMenu* WinnerMenu = nullptr;

	TSubclassOf<UUserWidget> WinnerWidgetClass = nullptr;
};



