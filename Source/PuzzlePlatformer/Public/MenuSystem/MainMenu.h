// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UWidget;
class UButton;
class UServerEntry;
class UPanelWidget;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(FObjectInitializer const& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void LoadMainMenu();

	UFUNCTION()
	void Quit();

private:
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuHostButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuJoinButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuQuitButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuJoinButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuBackButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList = nullptr;

	TSubclassOf<UUserWidget> ServerEntryClass = nullptr;

	TOptional<uint32> SelectedIndex;
};
