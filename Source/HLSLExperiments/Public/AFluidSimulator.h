// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AFluidSimulator.generated.h"

UCLASS()
class HLSLEXPERIMENTS_API AAFluidSimulator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAFluidSimulator();
    virtual ~AAFluidSimulator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    void AddSource(int inN, float* x, float* s, float dt);
    void Diffuse(int inN, int b, float* x, float* x0, float diff, float dt);
    void Advect(int inN, int b, float* d, float* d0, float* inU, float* inV, float dt);
    void Project(int inN, float* inU, float* inV, float* p, float* div);
    void SetBnd(int inN, int b, float* x);
    void FluidStep(int inN, float* inU, float* inV, float* inu_prev, float* inv_prev, float* indens, float* indens_prev, float visc, float diff, float dt);

    const int N = 64;

    float* u;
    float* v;
    float* u_prev;
    float* v_prev;
    float* dens;
    float* dens_prev;

    UPROPERTY(EditAnywhere)
    float Viscosity = 0.0001f;

    UPROPERTY(EditAnywhere)
    float Diffusion = 0.0001f;

    UPROPERTY(EditAnywhere)
    UTexture2D* FluidTexture;

    uint8* TextureData;

};
