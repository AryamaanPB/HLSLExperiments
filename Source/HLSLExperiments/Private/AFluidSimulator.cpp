// Fill out your copyright notice in the Description page of Project Settings.


#include "AFluidSimulator.h"

#define IX(i, j) ((i) + (N + 2) * (j))

// Sets default values
AAFluidSimulator::AAFluidSimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    u = new float[(N + 2) * (N + 2)]();
    v = new float[(N + 2) * (N + 2)]();
    u_prev = new float[(N + 2) * (N + 2)]();
    v_prev = new float[(N + 2) * (N + 2)]();
    dens = new float[(N + 2) * (N + 2)]();
    dens_prev = new float[(N + 2) * (N + 2)]();
}

AAFluidSimulator::~AAFluidSimulator()
{
    delete[] u;
    delete[] v;
    delete[] u_prev;
    delete[] v_prev;
    delete[] dens;
    delete[] dens_prev;
}

// Called when the game starts or when spawned
void AAFluidSimulator::BeginPlay()
{
	Super::BeginPlay();

    FluidTexture = UTexture2D::CreateTransient(N, N);
    FluidTexture->UpdateResource();
    TextureData = static_cast<uint8*>(FluidTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	
}

// Called every frame
void AAFluidSimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FluidStep(N, u, v, u_prev, v_prev, dens, dens_prev, Viscosity, Diffusion, DeltaTime);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            TextureData[4 * (i + N * j)] = dens[IX(i, j)] * 255;
            TextureData[4 * (i + N * j) + 1] = dens[IX(i, j)] * 255;
            TextureData[4 * (i + N * j) + 2] = dens[IX(i, j)] * 255;
            TextureData[4 * (i + N * j) + 3] = 255;
        }
    }

    FluidTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
    FluidTexture->UpdateResource();
}

void AAFluidSimulator::AddSource(int inN, float* x, float* s, float dt)
{
    int size = (inN + 2) * (inN + 2);
    for (int i = 0; i < size; i++) x[i] += dt * s[i];
}

void AAFluidSimulator::Diffuse(int inN, int b, float* x, float* x0, float diff, float dt)
{
    int i, j, k;
    float a = dt * diff * inN * inN;
    for (k = 0; k < 20; k++)
    {
        for (i = 1; i <= inN; i++)
        {
            for (j = 1; j <= inN; j++)
            {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                    x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
            }
        }
        SetBnd(inN, b, x);
    }
}

void AAFluidSimulator::Advect(int inN, int b, float* d, float* d0, float* inU, float* inV, float dt)
{
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt * inN;
    for (i = 1; i <= inN; i++)
    {
        for (j = 1; j <= inN; j++)
        {
            x = i - dt0 * inU[IX(i, j)];
            y = j - dt0 * inV[IX(i, j)];
            if (x < 0.5f) x = 0.5f; if (x > N + 0.5f) x = N + 0.5f;
            if (y < 0.5f) y = 0.5f; if (y > N + 0.5f) y = N + 0.5f;
            i0 = (int)x; i1 = i0 + 1;
            j0 = (int)y; j1 = j0 + 1;
            s1 = x - i0; s0 = 1 - s1;
            t1 = y - j0; t0 = 1 - t1;
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    SetBnd(inN, b, d);
}

//TODO
void AAFluidSimulator::Project(int N, float* u, float* v, float* p, float* div)
{
    int i, j, k;
    float h = 1.0f / N;
    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            div[IX(i, j)] = -0.5f * h * (u[IX(i + 1, j)] - u[IX(i - 1, j)] +
                v[IX(i, j + 1)] - v[IX(i, j - 1)]);
            p[IX(i, j)] = 0;
        }
    }
    SetBnd(N, 0, div); SetBnd(N, 0, p);
    for (k = 0; k < 20; k++)
    {
        for (i = 1; i <= N; i++)
        {
            for (j = 1; j <= N; j++)
            {
                p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
                    p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
            }
        }
        SetBnd(N, 0, p);
    }
    for (i = 1; i <= N; i++)
    {
        for (j = 1; j <= N; j++)
        {
            u[IX(i, j)] -= 0.5f * N * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            v[IX(i, j)] -= 0.5f * N * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
        }
    }
    SetBnd(N, 1, u); SetBnd(N, 2, v);
}

void AAFluidSimulator::SetBnd(int N, int b, float* x)
{
    for (int i = 1; i <= N; i++)
    {
        x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N + 1)] = 0.5f * (x[IX(1, N + 1)] + x[IX(0, N)]);
    x[IX(N + 1, 0)] = 0.5f * (x[IX(N, 0)] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f * (x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void AAFluidSimulator::FluidStep(int N, float* u, float* v, float* u_prev, float* v_prev, float* dens, float* dens_prev, float visc, float diff, float dt)
{
    AddSource(N, dens, dens_prev, dt);
    AddSource(N, u, u_prev, dt);
    AddSource(N, v, v_prev, dt);

    std::swap(dens_prev, dens); Diffuse(N, 0, dens, dens_prev, diff, dt);
    std::swap(u_prev, u); Diffuse(N, 1, u, u_prev, visc, dt);
    std::swap(v_prev, v); Diffuse(N, 2, v, v_prev, visc, dt);

    Project(N, u, v, u_prev, v_prev);

    std::swap(dens_prev, dens); Advect(N, 0, dens, dens_prev, u, v, dt);
    std::swap(u_prev, u); Advect(N, 1, u, u_prev, u, v, dt);
    std::swap(v_prev, v); Advect(N, 2, v, v_prev, u, v, dt);

    Project(N, u, v, u_prev, v_prev);
}