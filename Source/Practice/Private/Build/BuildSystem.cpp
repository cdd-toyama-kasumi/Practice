// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/BuildSystem.h"

#include "Build/Floor.h"
#include "Character/MainCharacter.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

// Sets default values for this component's properties
UBuildSystem::UBuildSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuildSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildSystem::SetPlayer(AMainCharacter* Player)
{
	MainCharacter = Player;
}

void UBuildSystem::SetBuildItem()
{
	if(!BuildItem)
	{
		BuildItem = GetWorld()->SpawnActor<AFloor>(FVector(MainCharacter->GetActorLocation().X+100.f,MainCharacter->GetActorLocation().Y+100.f,MainCharacter->GetActorLocation().Z),FRotator::ZeroRotator);
		Cast<AFloor>(BuildItem)->SetCollision(ECollisionEnabled::QueryOnly);
	}
}

void UBuildSystem::UnSetBuildItem()
{
	if(BuildItem)
	{
		GetWorld()->DestroyActor(Cast<AFloor>(BuildItem));
		BuildItem = nullptr;
	}
}

bool UBuildSystem::Building()
{
	if(!BuildItem)
	{
		return false;
	}
	if(Cast<AFloor>(BuildItem)->IsBlock)
	{
		LogScreen(1.0f,"放置被阻挡");
		return false;
	}
	
	Cast<AFloor>(BuildItem)->MeshComponent->SetMobility(EComponentMobility::Stationary);
	Cast<AFloor>(BuildItem)->SetCollision(ECollisionEnabled::QueryAndPhysics);
	Cast<AFloor>(BuildItem)->SetMaterial(FloorMat);
	BuildItem = nullptr;
	return true;
}

