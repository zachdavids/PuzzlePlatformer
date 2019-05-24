// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"
#include "WinnerMenu.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UWinnerMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	void SetDisplayMessage(FText Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinnerText = nullptr;
};
