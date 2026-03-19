// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChar.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the player camera component used for the first-person view
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attach the camera to the character's skeletal mesh at the "Head" socket
	// This makes the camera move with the character's head
	PlayerCamComp->SetupAttachment(GetMesh(), "Head");

	// Allows the camera to rotate based on the player's controller input (mouse/gamepad)
	PlayerCamComp->bUsePawnControlRotation = true;

	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Timer handle used to control the stats timer
	FTimerHandle StatsTimerHandle;

	// Calls DecreaseStats every 2 seconds in a loop
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind player input (keyboard/controller) to character functions
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binds forward/backward movement input to the MoveForward function
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);

	// Binds left/right movement input (strafing) to the MoveRight function
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);

	// Binds vertical camera look movement (looking up and down)
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);

	// Binds horizontal camera rotation (turning left and right)
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	// Calls StartJump when the jump button is pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);

	// Calls StopJump when the jump button is released
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	// Binds the FindObject action to the corresponding function when the key is pressed
	PlayerInputComponent->BindAction("FindObject", IE_Pressed, this, &APlayerChar::FindObject);
}

// Handles forward and backward movement
void APlayerChar::MoveForward(float axisValue)
{
	// Gets the forward direction based on the player's current camera rotation
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	// Applies movement in that direction using the input value
	AddMovementInput(Direction, axisValue);
}

// Handles left and right movement (strafing)
void APlayerChar::MoveRight(float axisValue)
{
	// Gets the right direction based on the player's current camera rotation
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	// Applies movement in that direction using the input value
	AddMovementInput(Direction, axisValue);
}

// Starts the jump action when the jump key is pressed
void APlayerChar::StartJump()
{
	// Sets the jump flag to true so the character begins jumping
	bPressedJump = true;
}

// Stops the jump action when the jump key is released
void APlayerChar::StopJump()
{
	// Sets the jump flag to false to stop the jumping action
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
	// Stores information about what the line trace hits
	FHitResult HitResult;

	// Starting point of the trace (player camera location)
	FVector StartLocation = PlayerCamComp->GetComponentLocation();

	// Direction of the trace based on where the camera is facing
	// Multiplied to extend the trace distance
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;

	// End point of the trace
	FVector EndLocation = StartLocation + Direction;

	// Collision settings for the trace
	FCollisionQueryParams QueryParams;

	// Ignore the player character so the trace doesn't hit itself
	QueryParams.AddIgnoredActor(this);

	// Enables complex collision checking
	QueryParams.bTraceComplex = false;

	// Allows returning the face index of the hit surface
	QueryParams.bReturnFaceIndex = true;

	// Perform a line trace (raycast) from the camera forward
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// Try to cast the hit actor to a resource object
		AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

		// Draw a debug line to visualize the trace (for testing purposes)
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, false, 3.0f);

		// If the object hit is a valid resource
		if (HitResource)
		{
			// Get the name of the resource
			FString hitName = HitResource->resourceName;

			// Get how much of the resource can be collected
			int resourceValue = HitResource->resourceAmount;

			// Reduce the total amount of resource remaining in the object
			HitResource->totalResource = HitResource->totalResource - resourceValue;
			
			if (HitResource->totalResource >= resourceValue)
			{
				GiveResource(resourceValue, hitName);

				check(GEngine != nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, hitName + " Collected: " + FString::FromInt(HitResource->resourceAmount)); // Display the amount of resource collected
			}
			else
			{
				check(GEngine != nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));

			}
		}
	}
}

// Adjusts the player's health by the given amount
// Ensures the health value does not exceed the maximum value of 100
void APlayerChar::SetHealth(float amount)
{
	// Check that adding the amount will keep health below the max limit
	if (Health + amount < 100)
	{
		// Apply the change to the health value
		Health = Health + amount;
	}
}

// Adjusts the player's hunger by the given amount
// Prevents the hunger value from going above the maximum of 100
void APlayerChar::SetHunger(float amount)
{
	// Check that the new hunger value stays within the allowed range
	if (Hunger + amount < 100)
	{
		// Apply the change to the hunger value
		Hunger = Hunger + amount;
	}
}

// Adjusts the player's stamina by the given amount
// Ensures stamina does not exceed the maximum value of 100
void APlayerChar::SetStamina(float amount)
{
	// Verify the new stamina value will not exceed the limit
	if (Stamina + amount <= 100)
	{
		// Apply the change to stamina
		Stamina = Stamina + amount;
	}
}

// Gradually decreases or updates player survival stats over time
// Typically called repeatedly (for example, during Tick or a timer)
void APlayerChar::DecreaseStats()
{
	// If the player still has hunger remaining, decrease it slowly
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	// Restore some stamina over time
	SetStamina(10.0f);

	// If hunger has reached zero, begin damaging the player's health
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

void APlayerChar::GiveResource(float amount, FString resourceType)
{
	// Add the collected amount to the correct resource in the player's inventory
	if (resourceType == "Wood")
	{
		// Increase the amount of wood by the collected amount
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		// Increase the amount of stone by the collected amount
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		// Increase the amount of berries by the collected amount
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

