// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzleGameInstance.generated.h"

class UUserWidget;
class UMainMenu;
class UInGameMenu;
class FOnlineSessionSearch;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UPuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UPuzzleGameInstance(FObjectInitializer const& ObjectInitializer);

	virtual void Init();

	void NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

protected:
	UFUNCTION(exec)
	virtual void HostServer() override;

	UFUNCTION(exec)
	virtual void JoinServer(uint32 Index) override;

	UFUNCTION(exec)
	virtual void LoadMainMenu() override;

	UFUNCTION(exec)
	virtual void RefreshServerList() override;

private:
	TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> InGameMenuClass;

	UMainMenu* MainMenu = nullptr;
	UInGameMenu* InGameMenu = nullptr;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
