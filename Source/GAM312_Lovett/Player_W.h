// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_W.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_LOVETT_API UPlayer_W : public UUserWidget
{
	GENERATED_BODY()

public:

	// This function can be implemented in Blueprint (not in C++)
	// It is used to update the UI bars (health, hunger, stamina)
	UFUNCTION(BlueprintImplementableEvent)

	// Values passed to update the UI
	void UpdateBars(float Health1, float Hunger1, float Stamina1);
};
