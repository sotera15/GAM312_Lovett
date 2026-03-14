// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

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
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StatsTimerHandle;
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

