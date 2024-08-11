// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerController.h"
#include "ASnake.h"
#include "Kismet/GameplayStatics.h"

void ASnakePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Ensure the SnakeActor is set by finding it in the scene
    SnakeActor = Cast<ASnake>(UGameplayStatics::GetActorOfClass(GetWorld(), ASnake::StaticClass()));

    // Bind the input actions for the snake movement
    InputComponent->BindAction("W_Up", IE_Pressed, this, &ASnakePlayerController::MoveUp);
    InputComponent->BindAction("S_Down", IE_Pressed, this, &ASnakePlayerController::MoveDown);
    InputComponent->BindAction("A_Left", IE_Pressed, this, &ASnakePlayerController::MoveLeft);
    InputComponent->BindAction("D_Right", IE_Pressed, this, &ASnakePlayerController::MoveRight);
}

void ASnakePlayerController::MoveUp()
{
    if (SnakeActor)
    {
        // Move the snake up by one cell size in the normalized space
        SnakeActor->SetDirection(FVector(0.0f, SnakeActor->CellSize, 0.0f));
    }
}

void ASnakePlayerController::MoveDown()
{
    if (SnakeActor)
    {
        // Move the snake down by one cell size in the normalized space
        SnakeActor->SetDirection(FVector(0.0f, -SnakeActor->CellSize, 0.0f));
    }
}

void ASnakePlayerController::MoveLeft()
{
    if (SnakeActor)
    {
        // Move the snake left by one cell size in the normalized space
        SnakeActor->SetDirection(FVector(SnakeActor->CellSize, 0.0f, 0.0f));
    }
}

void ASnakePlayerController::MoveRight()
{
    if (SnakeActor)
    {
        // Move the snake right by one cell size in the normalized space
        SnakeActor->SetDirection(FVector(-SnakeActor->CellSize, 0.0f, 0.0f));
    }
}
