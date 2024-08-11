// Fill out your copyright notice in the Description page of Project Settings.


#include "ASnake.h"

// Sets default values
ASnake::ASnake()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set default values
    SnakeLength = 1;
    SnakeSpeed = 3.0f; // Adjusted speed to work within normalized space (0-1)
    SnakeHeadPosition = FVector(0.5f, 0.5f, 0.0f); // Start in the middle of the grid
    CurrentDirection = FVector(CellSize, 0.0f, 0.0f); // Movement in the x-direction

    // Initialize food position within normalized space (0 to 1)
    FoodPosition = FVector(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 0.0f);

    // Load the Material Parameter Collection asset
    static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MPCAsset(TEXT("/Script/Engine.MaterialParameterCollection'/Game/Snake/MP_Snake.MP_Snake'"));

    if (MPCAsset.Succeeded())
    {
        MaterialParameterCollection = MPCAsset.Object;
    }
}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Move(DeltaTime);

    // Check for collisions with the food
    if (CheckCollisionWithFood(FoodPosition))
    {
        // If there was a collision, spawn new food at a random position in normalized space
        FoodPosition = FVector(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 0.0f);
    }

    CheckSelfCollision(SnakeHeadPosition);

    // Get the collection instance from the world
    if (MaterialParameterCollection)
    {
        UMaterialParameterCollectionInstance* CollectionInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
        if (CollectionInstance)
        {
            CollectionInstance->SetScalarParameterValue("SnakeHeadX", SnakeHeadPosition.X);
            CollectionInstance->SetScalarParameterValue("SnakeHeadY", SnakeHeadPosition.Y);
            CollectionInstance->SetScalarParameterValue("FoodX", FoodPosition.X);
            CollectionInstance->SetScalarParameterValue("FoodY", FoodPosition.Y);
            if (!TailPositions.IsEmpty())
            {
                CollectionInstance->SetScalarParameterValue("TailPosX", TailPositions[0].X);
                CollectionInstance->SetScalarParameterValue("TailPosY", TailPositions[0].Y);
            }
        }
    }
}

// Function to move the snake
void ASnake::Move(float DeltaTime)
{
    FVector PreviousHeadPosition = SnakeHeadPosition;
    SnakeHeadPosition += CurrentDirection * SnakeSpeed * DeltaTime;

    // Ensure the tail positions are updated correctly
    if (TailPositions.Num() > 0)
    {
        for (int32 i = TailPositions.Num() - 1; i > 0; --i)
        {
            TailPositions[i] = TailPositions[i - 1];
        }
        TailPositions[0] = PreviousHeadPosition;
    }
}

// Check for collision with food
bool ASnake::CheckCollisionWithFood(FVector InFoodPosition)
{
    if (FVector::Dist(SnakeHeadPosition, InFoodPosition) < CellSize)
    {
        SnakeLength++;
        TailPositions.Add(SnakeHeadPosition); // Add the current head position to the tail

        // Log the tail positions for debugging
        UE_LOG(LogTemp, Warning, TEXT("Food collision detected! Adding to tail: %s"), *SnakeHeadPosition.ToString());
        for (const FVector& Pos : TailPositions)
        {
            UE_LOG(LogTemp, Warning, TEXT("Tail Position: %s"), *Pos.ToString());
        }

        return true;
    }
    return false;
}

void ASnake::CheckSelfCollision(const FVector& HeadPosition)
{
    // Loop through each segment of the snake's tail to check for collisions with the head
    for (const FVector& TailSegment : TailPositions)
    {
        if (FVector::Dist(HeadPosition, TailSegment) < CellSize)
        {
            // If the head touches the tail, reset the snake
            SnakeLength = 1;
            TailPositions.Empty();
            break;
        }
    }
}

// Set the direction of the snake
void ASnake::SetDirection(FVector NewDirection)
{
    CurrentDirection = NewDirection;
}