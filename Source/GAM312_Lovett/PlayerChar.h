// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "Buildingpart.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_LOVETT_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles forward and backward movement input from the player
	UFUNCTION()
	void MoveForward(float axisValue);

	// Handles left and right movement (strafing) input from the player
	UFUNCTION()
	void MoveRight(float axisValue);

	// Called when the player presses the jump input to begin jumping
	UFUNCTION()
	void StartJump();

	// Called when the player releases the jump input to stop jumping
	UFUNCTION()
	void StopJump();

	// Attempts to locate or interact with an object in front of the player
	UFUNCTION()
	void FindObject();

	// Camera component attached to the player character for the player's view
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	// Player's health value (max default is 100)
	// Can be edited in the editor and accessed in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	// Player's hunger level (used for survival mechanics)
	// Can be edited in the editor and accessed in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	// Player's stamina value (used for actions like running or jumping)
	// Can be edited in the editor and accessed in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// Amount of wood the player currently has
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	// Amount of stone the player currently has
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	// Amount of berries the player currently has
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// Array storing the quantities of each resource
	// Can be accessed and modified in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// Array storing the names of the resources
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// Decal material used to show a hit effect
	// Set this in the editor
	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;


	// Stores the amount of each building resource the player has
	// (ex: wood, stone, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
	TArray<int> BuildingArray;

	// Tracks whether the player is currently in building mode
	UPROPERTY()
	bool isBuilding;

	// Class reference for the building part to spawn
	// Set this in the editor or Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABuildingPart> BuildPartClass;

	// Reference to the currently spawned building part
	// Used for placing or updating the object
	UPROPERTY()
	ABuildingPart* spawnedPart;

	// Function to increase or modify the player's health by a given amount
	// Can be called from Blueprints
	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);

	// Function to increase or modify the player's hunger by a given amount
	// Can be called from Blueprints
	UFUNCTION(BlueprintCallable)
	void SetHunger(float amount);

	// Function to modify the player's stamina
	// Can be called from Blueprints
	UFUNCTION(BlueprintCallable)
	void SetStamina(float amount);

	// Function used to decrease multiple player stats over time (such as hunger or stamina)
	// Often used for survival mechanics like gradual stat decay
	UFUNCTION()
	void DecreaseStats();


	// Adds a specified amount of a resource to the player (based on type)
	UFUNCTION()
	void GiveResource(float amount, FString resourceType);

	// Updates resource values when building (subtracts required materials)
	UFUNCTION(BlueprintCallable)
	void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	// Spawns a building part based on an ID
	// Returns whether the spawn was successful
	UFUNCTION(BlueprintCallable)
	void SpawnBuilding(int buildingID, bool& isSuccess);

	// Rotates the currently spawned building before placement
	UFUNCTION()
	void RotateBuilding();
};
