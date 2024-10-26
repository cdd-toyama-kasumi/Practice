// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Floor.h"

#include "Components/BoxComponent.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

bool IsSideContain(const FString& Value)
{
	return Value.Contains("Left") || Value.Contains("Right") || Value.Contains("Up") || Value.Contains("Down");
}

bool IsTypeContain(const FString& Value)
{
	return Value.Contains("Floor") || Value.Contains("Wall");
}

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
	BoxComponent->InitBoxExtent(FVector(HalfSizeXY, HalfSizeXY, HalfSizeZ));

	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("RightBox");
	RightSideBoxComponent->SetupAttachment(MeshComponent);
	RightSideBoxComponent->InitBoxExtent(FVector(10,HalfSizeXY/2, HalfSizeZ*2));
	RightSideBoxComponent->SetRelativeLocation(FVector(HalfSizeXY,0,0));

	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	LeftSideBoxComponent->SetupAttachment(MeshComponent);
	LeftSideBoxComponent->InitBoxExtent(FVector(10,HalfSizeXY/2, HalfSizeZ*2));
	LeftSideBoxComponent->SetRelativeLocation(FVector(-HalfSizeXY,0,0));
	
	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("UpBox");
	UpSideBoxComponent->SetupAttachment(MeshComponent);
	UpSideBoxComponent->InitBoxExtent(FVector(HalfSizeXY/2,10, HalfSizeZ*2));
	UpSideBoxComponent->SetRelativeLocation(FVector(0,-HalfSizeXY,0));
	
	DownSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownSideBoxComponent->SetupAttachment(MeshComponent);
	DownSideBoxComponent->InitBoxExtent(FVector(HalfSizeXY/2,10, HalfSizeZ*2));
	DownSideBoxComponent->SetRelativeLocation(FVector(0,HalfSizeXY,0));

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnBeginOverLap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::OnEndOverLap);

	RightSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::RightSideBeginOverLap);
	RightSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::RightSideEndOverLap);

	LeftSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::LeftSideBeginOverLap);
	LeftSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::LeftSideEndOverLap);

	UpSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::UpSideBeginOverLap);
	UpSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::UpSideEndOverLap);

	DownSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::DownSideBeginOverLap);
	DownSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloor::DownSideEndOverLap);
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

void AFloor::SetBlur(float Emissive,FColor Color)
{
	MeshComponent->SetScalarParameterValueOnMaterials("emissive",Emissive);
	MeshComponent->SetVectorParameterValueOnMaterials("blur",FVector(Color.R,Color.G,Color.B));
}

void AFloor::SetMaterial(FString Material)
{
	UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *Material);
	MeshComponent->SetMaterial(0, MaterialInterface);
}

void AFloor::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LogScreen(2, "OnBeginOverLap" + OtherActor->GetName());
	BlockActorName = OtherActor->GetName();
	if(MeshComponent->Mobility == EComponentMobility::Type::Movable && !ForceBuild)
	{
		SetBlur(0.1,FColor::Red);
	}
	IsBlock = true;
}

void AFloor::OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LogScreen(2, "OnEndOverLap" + OtherActor->GetName());
	BlockActorName = "";
	if(MeshComponent->Mobility == EComponentMobility::Type::Movable)
	{
		SetBlur(0.1,FColor::Green);
	}
	IsBlock = false;
}

void AFloor::RightSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Right Begin");
		ArraySide[static_cast<int32>(Direction::Right)] = true;
	}
}

void AFloor::RightSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Right End");
		ArraySide[static_cast<int32>(Direction::Right)] = false;
	}
}

void AFloor::LeftSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Left Begin");
		ArraySide[static_cast<int32>(Direction::Left)] = true;
	}
}

void AFloor::LeftSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Left End");
		ArraySide[static_cast<int32>(Direction::Left)] = false;
	}
}

void AFloor::UpSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Up Begin");
		ArraySide[static_cast<int32>(Direction::Up)] = true;
	}
}

void AFloor::UpSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Up End");
		ArraySide[static_cast<int32>(Direction::Up)] = false;
	}
}

void AFloor::DownSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Down Begin");
		ArraySide[static_cast<int32>(Direction::Down)] = true;
	}
}

void AFloor::DownSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		LogScreen(10,"Down End");
		ArraySide[static_cast<int32>(Direction::Down)] = false;
	}
}

bool AFloor::Save(FString Name, FString CompName)
{
	if(IsSet || !IsTypeContain(Name) || Name == GetName() || !IsSideContain(CompName))
	{
		return false;
	}
	LogDisplay("%s %s",*Name, *CompName);
	IsAttach = true;
	BlockActorName = Name;
	BlockActorSide = CompName;
	FAttachActor Actor;
	Actor.Name = Name;
	Actor.Side = CompName;
	AttachActorCache.Emplace(Actor);
	return true;
}

bool AFloor::Remove(FString Name, FString CompName)
{
	if(IsSet || !IsTypeContain(Name) || Name == GetName() || AttachActorCache.IsEmpty() || !IsSideContain(CompName))
	{
		return false;
	}
	LogDisplay("%s %s",*Name, *CompName);
	IsAttach = false;
	BlockActorName = nullptr;
	BlockActorSide = nullptr;
	for(int32 i = 0; i < AttachActorCache.Num(); ++i)
	{
		if(AttachActorCache[i].Name == Name && AttachActorCache[i].Side == CompName)
		{
			AttachActorCache.RemoveAt(i);
		}
	}
	return true;
}

