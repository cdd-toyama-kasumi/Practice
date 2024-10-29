// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Wall.h"

#include "Components/BoxComponent.h"
#include "Tools/Static.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *FloorMesh));
	SetRootComponent(MeshComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->InitBoxExtent(FVector(HalfSizeX, HalfSizeY, HalfSizeZ));

	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("RightBox");
	RightSideBoxComponent->SetupAttachment(MeshComponent);
	RightSideBoxComponent->InitBoxExtent(FVector(10,HalfSizeY, HalfSizeZ/2));
	RightSideBoxComponent->SetRelativeLocation(FVector(HalfSizeX,0,0));

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	LeftSideBoxComponent->SetupAttachment(MeshComponent);
	LeftSideBoxComponent->InitBoxExtent(FVector(10,HalfSizeY, HalfSizeZ/2));
	LeftSideBoxComponent->SetRelativeLocation(FVector(-HalfSizeX,0,0));
	
	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("UpBox");
	UpSideBoxComponent->SetupAttachment(MeshComponent);
	UpSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2,HalfSizeY/2, 10));
	UpSideBoxComponent->SetRelativeLocation(FVector(0,0,HalfSizeZ));

	DownSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownSideBoxComponent->SetupAttachment(MeshComponent);
	DownSideBoxComponent->InitBoxExtent(FVector(HalfSizeX/2,HalfSizeY/2, 10));
	DownSideBoxComponent->SetRelativeLocation(FVector(0,0,-HalfSizeZ));
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

