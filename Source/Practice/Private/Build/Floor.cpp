// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Floor.h"

#include "Components/BoxComponent.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *FloorMesh));
	SetRootComponent(MeshComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->InitBoxExtent(FVector(SizeXY, SizeXY, SizeZ));
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnBeginOverLap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::OnEndOverLap);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloor::SetCollision(ECollisionEnabled::Type Type)
{
	MeshComponent->SetCollisionEnabled(Type);
	if(Type == ECollisionEnabled::QueryOnly)
	{
		MeshComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	}
	else
	{
		MeshComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	}
}

void AFloor::SetMaterial(FString Material)
{
	UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *Material);
	MeshComponent->SetMaterial(0, MaterialInterface);
}

void AFloor::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LogScreen(2, "OnBeginOverLap" + OtherActor->GetName());
	MeshComponent->SetVectorParameterValueOnMaterials("BaseColor",FVector(FLinearColor::Red.R,FLinearColor::Red.G,FLinearColor::Red.B));
	IsBlock = true;
}

void AFloor::OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	LogScreen(2, "OnEndOverLap" + OtherActor->GetName());
	MeshComponent->SetVectorParameterValueOnMaterials("BaseColor",FVector(FLinearColor::Green.R,FLinearColor::Green.G,FLinearColor::Green.B));
	IsBlock = false;
}

