// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntry.generated.h"

class UTextBlock;
class UMainMenu;
class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMER_API UServerEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HostName = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ServerCapacity = nullptr;

	void Setup(UMainMenu* Parent, uint32 Index);

protected:
	uint32 Index;

	UPROPERTY(Meta = (BindWidget))
	UButton* SelectionButton = nullptr;

	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	UMainMenu* Parent = nullptr;
};
