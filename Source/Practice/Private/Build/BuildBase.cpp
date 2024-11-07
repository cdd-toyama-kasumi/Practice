// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/BuildBase.h"

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
ABuildBase::ABuildBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(MeshComponent);

	RightSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("RightBox");
	RightSideBoxComponent->SetupAttachment(MeshComponent);
	
	LeftSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	LeftSideBoxComponent->SetupAttachment(MeshComponent);
	
	UpSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("UpBox");
	UpSideBoxComponent->SetupAttachment(MeshComponent);
	
	DownSideBoxComponent = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownSideBoxComponent->SetupAttachment(MeshComponent);
	
}

// Called when the game starts or when spawned
void ABuildBase::BeginPlay()
{
	Super::BeginPlay();
	SetCollision(ECollisionEnabled::QueryOnly);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildBase::OnBeginOverLap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildBase::OnEndOverLap);

	RightSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildBase::RightSideBeginOverLap);
	RightSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildBase::RightSideEndOverLap);

	LeftSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildBase::LeftSideBeginOverLap);
	LeftSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildBase::LeftSideEndOverLap);
	
	UpSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildBase::UpSideBeginOverLap);
	UpSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildBase::UpSideEndOverLap);

	DownSideBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuildBase::DownSideBeginOverLap);
	DownSideBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABuildBase::DownSideEndOverLap);
}

// Called every frame
void ABuildBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildBase::OnBlurAttach(UBuildSystem* Caller)
{
	
}

void ABuildBase::SetCollision(ECollisionEnabled::Type Type)
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

void ABuildBase::SetBlur(float Emissive,FColor Color)
{
	MeshComponent->SetScalarParameterValueOnMaterials("emissive",Emissive);
	MeshComponent->SetVectorParameterValueOnMaterials("blur",FVector(Color.R,Color.G,Color.B));
}


void ABuildBase::SetMaterial(const FString& Material)
{
	UMaterialInterface* MaterialInterface = LoadObject<UMaterialInterface>(nullptr, *Material);
	MeshComponent->SetMaterial(0, MaterialInterface);
}

void ABuildBase::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LogScreen(2, "OnBeginOverLap" + OtherActor->GetName());
	BlockActorName = OtherActor->GetName();
	if(MeshComponent->Mobility == EComponentMobility::Type::Movable && !ForceBuild)
	{
		SetBlur(0.1,FColor::Red);
	}
	IsBlock = true;
}

void ABuildBase::OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//LogScreen(2, "OnEndOverLap" + OtherActor->GetName());
	BlockActorName = "";
	if(MeshComponent->Mobility == EComponentMobility::Type::Movable)
	{
		SetBlur(0.1,FColor::Green);
	}
	IsBlock = false;
}

void ABuildBase::RightSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Right Begin");
		ArraySide[static_cast<int32>(Direction::Right)] = true;
	}
}

void ABuildBase::RightSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Right End");
		ArraySide[static_cast<int32>(Direction::Right)] = false;
	}
}

void ABuildBase::LeftSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Left Begin");
		ArraySide[static_cast<int32>(Direction::Left)] = true;
	}
}

void ABuildBase::LeftSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Left End");
		ArraySide[static_cast<int32>(Direction::Left)] = false;
	}
}

void ABuildBase::UpSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Up Begin");
		ArraySide[static_cast<int32>(Direction::Up)] = true;
	}
}

void ABuildBase::UpSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Up End");
		ArraySide[static_cast<int32>(Direction::Up)] = false;
	}
}

void ABuildBase::DownSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Save(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Down Begin");
		ArraySide[static_cast<int32>(Direction::Down)] = true;
	}
}

void ABuildBase::DownSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Remove(OtherActor->GetName(),OtherComp->GetName()))
	{
		//LogScreen(10,"Down End");
		ArraySide[static_cast<int32>(Direction::Down)] = false;
	}
}

bool ABuildBase::Save(FString Name, FString CompName)
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

bool ABuildBase::Remove(FString Name, FString CompName)
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
