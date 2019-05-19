// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	if (!ensure(Super::Initialize())) return false;

	MainMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	MainMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	MainMenuQuitButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);
	JoinMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenu::LoadMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface)) return;
	MenuInterface->HostServer();
}

void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface)) return;
	if (!ensure(AddressTextBox)) return;
	MenuInterface->JoinServer(AddressTextBox->GetText().ToString());
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::LoadMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	PlayerController->ConsoleCommand("quit");
}
