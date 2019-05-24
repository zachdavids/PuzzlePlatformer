// Fill out your copyright notice in the Description page of Project Settings.


#include "WinnerMenu.h"
#include "Components/TextBlock.h"

void UWinnerMenu::SetDisplayMessage(FText Text)
{
	WinnerText->SetText(Text);
}
