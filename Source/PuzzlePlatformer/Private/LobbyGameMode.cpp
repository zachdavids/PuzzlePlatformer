// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzleGameInstance.h"
#include "Public/TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumPlayers;
	if (NumPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(LobbyTimerHandle, this, &ALobbyGameMode::StartGame, 10.0f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumPlayers;
}

void ALobbyGameMode::StartGame()
{
	UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	if (!ensure(GameInstance)) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Levels/Level1?listen");
}
