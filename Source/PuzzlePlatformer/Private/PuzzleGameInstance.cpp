// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleGameInstance.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "MenuSystem/MenuInterface.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

UPuzzleGameInstance::UPuzzleGameInstance(FObjectInitializer const& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructed"));

	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuWBP.Class)) return;
	MenuClass = MainMenuWBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuWBP.Class)) return;
	InGameMenuClass = InGameMenuWBP.Class;
}

void UPuzzleGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Initialized"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!ensure(Subsystem)) return;
	UE_LOG(LogTemp, Warning, TEXT("Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();
	if (!ensure(SessionInterface)) return;
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;
	Engine->OnNetworkFailure().AddUObject(this, &UPuzzleGameInstance::NetworkError);
}

void UPuzzleGameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPuzzleGameInstance::HostServer()
{
	if (!ensure(SessionInterface)) return;
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(FName("Game Session"));

	if (!ExistingSession)
	{
		CreateSession();
	}
	else
	{
		SessionInterface->DestroySession(FName("Game Session"));
	}
}

void UPuzzleGameInstance::JoinServer(FString const& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Yellow, FString(TEXT("Joining: ")) + Address);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzleGameInstance::CreateSession()
{
	if (!ensure(SessionInterface)) return;
	FOnlineSessionSettings SessionSettings;
	SessionInterface->CreateSession(0, FName("Game Session"), SessionSettings);
}

void UPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Creation Failed"));
	}
	else
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine)) return;
		Engine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Yellow, TEXT("Hosting"));

		UWorld* World = GetWorld();
		if (!ensure(World)) return;
		World->ServerTravel("/Game/Levels/Level1?listen");
	}
}

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UPuzzleGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Yellow, TEXT("Load Main Menu"));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) return;
	PlayerController->ClientTravel("/Game/Levels/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzleGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) return;
	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzleGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass)) return;
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}
