// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "MenuInterface.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMenuWidget::Setup()
{
	this->bIsFocusable = true;
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeUIOnly UIInputMode;
	UIInputMode.SetWidgetToFocus(this->TakeWidget());
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(UIInputMode);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeGameOnly GameInputMode;
	PlayerController->SetInputMode(GameInputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeGameOnly GameInputMode;

	PlayerController->SetInputMode(GameInputMode);
	PlayerController->bShowMouseCursor = false;
}
