// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_LOVETT_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Name of the resource (example: Wood, Stone, Berry)
	UPROPERTY(EditAnywhere)
		FString resourceName = "Wood";

	// Amount of the resource the player receives when collected
	UPROPERTY(EditAnywhere)
		int resourceAmount = 5;

	// Total amount of this resource available in the world object
	UPROPERTY(EditAnywhere)
		int totalResource = 100;

	// Temporary text variable used when updating the resource display
	UPROPERTY()
		FText tempText;

	// Text component used to display the resource name in the world
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameTxt;

	// Static mesh representing the resource object in the world
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

};
