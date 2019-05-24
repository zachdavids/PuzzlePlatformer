// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformerGameMode.h"
#include "PuzzlePlatformerCharacter.h"
#include "WinnerMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/TimerManager.h"

APuzzlePlatformerGameMode::APuzzlePlatformerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/AdvancedLocomotionV3/Blueprints/MannequinCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> WinnerWidgetWBP(TEXT("/Game/MenuSystem/WBP_Winner"));
	if (WinnerWidgetWBP.Class != NULL)
	{
		WinnerWidgetClass = WinnerWidgetWBP.Class;
	}
}

void APuzzlePlatformerGameMode::EndRound(FString WinnerName)
{
	//if (WinnerWidgetClass != NULL)
	//{
	//	UWorld* World = GetWorld();
	//	if (!ensure(World)) return;

	//	APlayerController* Player = World->GetFirstPlayerController();
	//	WinnerMenu = CreateWidget<UWinnerMenu>(Player, WinnerWidgetClass);
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetName());
	//	if (Player->GetName().Compare(WinnerName))
	//	{ 
	//		WinnerMenu->SetDisplayMessage(FText::FromString(FString::Printf(TEXT("Congratulations %s! You have won!"), *Player->GetName())));
	//	}
	//	else
	//	{
	//		WinnerMenu->SetDisplayMessage(FText::FromString(FString::Printf(TEXT("%s has won!"), *Player->GetName())));
	//	}
	//	WinnerMenu->Setup();
	//}
	GetWorldTimerManager().SetTimer(EndRoundTimerHandle, this, &APuzzlePlatformerGameMode::ResetLevel, 5.0f);
}

void APuzzlePlatformerGameMode::ResetLevel()
{
	Super::ResetLevel();

	//WinnerMenu->TearDown();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Levels/Level1?listen");
}
