// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Floor.h"

#include "Components/BoxComponent.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

// Sets default values`
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HalfSizeX = 100.f;
	HalfSizeY = 100.f;
	HalfSizeZ = 5.f;
	
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *FloorMesh));
	
	BoxComponent->InitBoxExtent(FVector(HalfSizeX, HalfSizeY, HalfSizeZ));

	UpSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2, HalfSizeY/10, HalfSizeZ*2));
	UpSideBoxComponent->SetRelativeLocation(FVector(0, -HalfSizeY, 0));
	
	DownSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2, HalfSizeY/10, HalfSizeZ*2));
	DownSideBoxComponent->SetRelativeLocation(FVector(0, HalfSizeY, 0));
	
	LeftSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/10, HalfSizeY/2, HalfSizeZ*2));
    LeftSideBoxComponent->SetRelativeLocation(FVector(-HalfSizeX, 0, 0));
	
	RightSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/10, HalfSizeY/2, HalfSizeZ*2));
	RightSideBoxComponent->SetRelativeLocation(FVector(HalfSizeX, 0, 0));
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
