// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "BuildingPart.generated.h"

UCLASS()
class GAM312_LOVETT_API ABuildingPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh component that represents the visual model of the building part
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	// Arrow component used as a pivot/forward direction for placement and rotation
	UPROPERTY(EditAnywhere)
		UArrowComponent* PivotArrow;
};
