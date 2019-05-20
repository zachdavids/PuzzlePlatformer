// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "ServerEntry.h"
#include "components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu(FObjectInitializer const& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerEntryWBP(TEXT("/Game/MenuSystem/WBP_ServerEntry"));
	if (!ensure(ServerEntryWBP.Class)) return;
	ServerEntryClass = ServerEntryWBP.Class;
}

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
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue());
	}

	if (!ensure(MenuInterface)) return;
	//if (!ensure(AddressTextBox)) return;
	MenuInterface->JoinServer("");
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (!ensure(ServerEntryClass)) return;
	if (!ensure(ServerList)) return;

	ServerList->ClearChildren();
	for (int i = 0; i != ServerNames.Num(); ++i)
	{
		UServerEntry* ServerEntry = CreateWidget<UServerEntry>(this, ServerEntryClass);
		ServerEntry->ServerName->SetText(FText::FromString(ServerNames[i]));
		ServerEntry->Setup(this, i);
		ServerList->AddChild(ServerEntry);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (!ensure(MenuInterface)) return;
	MenuInterface->RefreshServerList();
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
