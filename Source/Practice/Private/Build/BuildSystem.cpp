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
	BlurAttach();
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
		BuildItem = GetWorld()->SpawnActor<AFloor>(FVector(0,0,-10000),FRotator::ZeroRotator);
		Cast<AFloor>(BuildItem)->SetBlur(0.1,FColor::Green);
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

void UBuildSystem::BlurAttach()
{
	if(BuildItem)
	{
		FRotator ViewRotation = MainCharacter->GetController()->GetControlRotation();
		FVector MainLocation = MainCharacter->GetActorLocation();
		float AngleControllerPlayer = 90.0f - (360.0f - ViewRotation.Pitch);
		if(AngleControllerPlayer <= 0.0f)
		{
			AngleControllerPlayer = 89.0f;
		}
		/*
		 *camera
		 *       ↘-------------------------------
		 *     z p ↘  AngleControllerPlayer
		 *       p θ ↘
		 *       p     ↘
		 *       p      ↘
		 *       --------- distance = z*tanθ
		 */
		BuildDistance = FMath::Tan(FMath::DegreesToRadians(AngleControllerPlayer)) * MainLocation.Z;
		if(BuildDistance > 600.0f)
		{
			BuildDistance = 600.0f;
		}
		float X = BuildDistance * FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw));
		float Y = BuildDistance * FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw));
		BuildLocation = FVector(MainLocation.X,MainLocation.Y,MainLocation.Z-50.0f) + FVector(X,Y,0);
		
		FString Msg = FString::Printf(TEXT("Roll:%f Pitch:%f Yaw:%f Distance:%f X:%f, Y:%f"), ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw,BuildDistance,BuildLocation.X,BuildLocation.Y);
		LogScreen(1.0f,Msg);

		Cast<AFloor>(BuildItem)->SetActorLocation(BuildLocation);
		Cast<AFloor>(BuildItem)->SetActorRotation(FRotator(0,ViewRotation.Yaw,0));

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
		LogScreen(1.0f,FString::Printf(TEXT("%ls:Building is blocked"), *BuildItem->GetName()));
		return false;
	}
	
	Cast<AFloor>(BuildItem)->MeshComponent->SetMobility(EComponentMobility::Stationary);
	Cast<AFloor>(BuildItem)->SetCollision(ECollisionEnabled::QueryAndPhysics);
	Cast<AFloor>(BuildItem)->SetBlur(0.0,FColor::Black);
	//Cast<AFloor>(BuildItem)->SetMaterial(FloorMat);
	BuildItem = nullptr;
	return true;
}

