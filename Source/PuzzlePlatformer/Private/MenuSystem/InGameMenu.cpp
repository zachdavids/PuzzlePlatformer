// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!ensure(Super::Initialize())) return false;

	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancel);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::Quit);

	return true;
}

void UInGameMenu::Cancel()
{
	TearDown();
}

void UInGameMenu::Quit()
{
	if (!ensure(MenuInterface)) return;
	MenuInterface->LoadMainMenu();
}
