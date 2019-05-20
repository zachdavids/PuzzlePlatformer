// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerEntry.h"
#include "MainMenu.h"
#include "Components/Button.h"

void UServerEntry::Setup(UMainMenu* Parent, uint32 Index)
{
	this->Parent = Parent;
	this->Index = Index;
	SelectionButton->OnClicked.AddDynamic(this, &UServerEntry::OnClicked);
}

void UServerEntry::OnClicked()
{
	Parent->SelectIndex(Index);
}
