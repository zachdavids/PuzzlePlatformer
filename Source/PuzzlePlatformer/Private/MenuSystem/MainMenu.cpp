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

	MainMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	MainMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	MainMenuQuitButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);
	JoinMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	JoinMenuBackButton->OnClicked.AddDynamic(this, &UMainMenu::LoadMainMenu);
	HostMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	HostMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::LoadMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface)) return;
	FString ServerName = HostMenuTextBox->Text.ToString();
	MenuInterface->HostServer(ServerName);
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet())
	{
		if (!ensure(MenuInterface)) return;
		MenuInterface->JoinServer(SelectedIndex.GetValue());
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerDataList)
{
	if (!ensure(ServerEntryClass)) return;
	if (!ensure(ServerList)) return;

	ServerList->ClearChildren();
	for (int i = 0; i != ServerDataList.Num(); ++i)
	{
		UServerEntry* ServerEntry = CreateWidget<UServerEntry>(this, ServerEntryClass);
		ServerEntry->ServerName->SetText(FText::FromString(ServerDataList[i].Name));
		ServerEntry->HostName->SetText(FText::FromString(ServerDataList[i].HostName));
		ServerEntry->ServerCapacity->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), 
			ServerDataList[i].CurrentPlayers, ServerDataList[i].TotalPlayers)));
		ServerEntry->Setup(this, i);
		ServerList->AddChild(ServerEntry);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}


void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(HostMenu)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
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

void UMainMenu::UpdateChildren()
{
	if (!SelectedIndex.IsSet()) return;

	for (int i = 0; i != ServerList->GetChildrenCount(); ++i)
	{
		UServerEntry* Entry = Cast<UServerEntry>(ServerList->GetChildAt(i));
		if (SelectedIndex.GetValue() == i)
		{
			Entry->bSelected = true;
			Entry->SetColorAndOpacity(FLinearColor::Green);
		}
		else
		{
			Entry->bSelected = false;
			Entry->SetColorAndOpacity(FLinearColor::White);
		}
	}
}
