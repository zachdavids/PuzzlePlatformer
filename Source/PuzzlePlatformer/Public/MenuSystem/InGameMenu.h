// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void Cancel();

	UFUNCTION()
	void Quit();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton = nullptr;
};
