// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SnakePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HLSLEXPERIMENTS_API ASnakePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void SetupInputComponent() override;

private:
    UPROPERTY()
    class ASnake* SnakeActor;

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
	
};
