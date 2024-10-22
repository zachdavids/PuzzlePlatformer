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

const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzleGameInstance::UPuzzleGameInstance(FObjectInitializer const& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuWBP.Class)) return;
	MenuClass = MainMenuWBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuWBP.Class)) return;
	InGameMenuClass = InGameMenuWBP.Class;
}

void UPuzzleGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!ensure(Subsystem)) return;
	UE_LOG(LogTemp, Warning, TEXT("Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();
	if (!ensure(SessionInterface)) return;
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnJoinSessionComplete);

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;
	Engine->OnNetworkFailure().AddUObject(this, &UPuzzleGameInstance::NetworkError);
}

void UPuzzleGameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}


void UPuzzleGameInstance::StartSession()
{
	if (!ensure(SessionInterface)) return;
	SessionInterface->StartSession(NAME_GameSession);
}

void UPuzzleGameInstance::HostServer(FString ServerName)
{
	if (!ensure(SessionInterface)) return;

	this->ServerName = ServerName;

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);

	if (!ExistingSession)
	{
		CreateSession();
	}
	else
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
}

void UPuzzleGameInstance::JoinServer(uint32 Index)
{
	if (!ensure(SessionInterface)) return;
	if (!ensure(SessionSearch)) return;

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}

void UPuzzleGameInstance::CreateSession()
{
	if (!ensure(SessionInterface)) return;
	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, ServerName, 
		EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
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
		World->ServerTravel("/Game/Levels/Lobby?listen");
	}
}

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UPuzzleGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	if (!ensure(SessionSearch)) return;

	if (bSuccess)
	{
		TArray<FServerData> ServerNames;
		for (auto Result : SessionSearch->SearchResults)
		{
			FServerData Data;
			FString ServerSetName;
			Data.Name = (Result.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerSetName)) ? ServerSetName : "No server name";
			Data.HostName = Result.Session.OwningUserName;
			Data.TotalPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.TotalPlayers - Result.Session.NumOpenPublicConnections;


			ServerNames.Add(Data);
		}
		MainMenu->SetServerList(ServerNames);
	}
}


void UPuzzleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!ensure(SessionInterface)) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection String Not Found"));
	}
	
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController)) return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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


void UPuzzleGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!ensure(SessionSearch)) return;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
