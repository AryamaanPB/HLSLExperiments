// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ASnake.generated.h"

UCLASS()
class HLSLEXPERIMENTS_API ASnake : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASnake();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Functions to handle snake movement and food collision
    void Move(float DeltaTime);
    bool CheckCollisionWithFood(FVector FoodPosition);
    void CheckSelfCollision(const FVector& HeadPosition);

    // Function to set direction
    void SetDirection(FVector NewDirection);

public:
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> TailPositions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake")
    float TimeTolerance = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake")
    float CellSize = 0.0333f; // Assuming a 30x30 grid, CellSize = 1 / 30

private:
    FVector CurrentDirection;
    FVector PrevDirection;

    float TotalTime = 0.0f;

    int32 SnakeLength;
    float SnakeSpeed;
    FVector SnakeHeadPosition;
    FVector FoodPosition;
    UMaterialParameterCollection* MaterialParameterCollection;

public:
    // Size of each grid cell in normalized space
    //const float CellSize = 0.0333f; // Assuming a 30x30 grid, CellSize = 1 / 30
};
