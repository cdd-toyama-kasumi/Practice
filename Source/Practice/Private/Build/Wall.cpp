// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Wall.h"

#include "Components/BoxComponent.h"
#include "Tools/Static.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *WallMesh));

	BoxComponent->InitBoxExtent(FVector(HalfSizeX, HalfSizeY, HalfSizeZ));

	UpSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2, HalfSizeY*2, HalfSizeZ/10));
	UpSideBoxComponent->SetRelativeLocation(FVector(0, 0, HalfSizeZ));
	
	DownSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2, HalfSizeY*2, HalfSizeZ/10));
	DownSideBoxComponent->SetRelativeLocation(FVector(0, 0, -HalfSizeZ));
	
	LeftSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/10, HalfSizeY*2, HalfSizeZ/2));
	LeftSideBoxComponent->SetRelativeLocation(FVector(-HalfSizeX, 0, 0));
	
	RightSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/10, HalfSizeY*2, HalfSizeZ/2));
	RightSideBoxComponent->SetRelativeLocation(FVector(HalfSizeX, 0, 0));
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

