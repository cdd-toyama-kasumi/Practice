// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Floor.h"

#include "Build/BuildSystem.h"
#include "Character/MainCharacter.h"
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
	
	BoxComponent->InitBoxExtent(FVector(HalfSizeX-HalfSizeX/10, HalfSizeY-HalfSizeY/10, HalfSizeZ));

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

void AFloor::OnBlurAttach(UBuildSystem* Caller)
{
		FRotator ViewRotation = Caller->MainCharacter->GetController()->GetControlRotation();
		FVector MainLocation = Caller->MainCharacter->GetActorLocation();
		float AngleControllerPlayer = (ViewRotation.Pitch > 270  && ViewRotation.Pitch < 360) ? ViewRotation.Pitch - 270 : 89;
		/*
		 *camera
		 *       ↘-------------------------------
		 *     z p ↘  AngleControllerPlayer
		 *       p θ ↘
		 *       p     ↘
		 *       p      ↘
		 *       --------- distance = z*tanθ
		 */
		Caller->BuildDistance = FMath::Tan(FMath::DegreesToRadians(AngleControllerPlayer)) * MainLocation.Z;
		if(Caller->BuildDistance > 600.0f)
		{
			Caller->BuildDistance = 600.0f;
		}
		float X = Caller->BuildDistance * FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw));
		float Y = Caller->BuildDistance * FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw));
		Caller->BuildLocation = FVector(MainLocation.X,MainLocation.Y,MainLocation.Z-50.0f) + FVector(X,Y,0);
		Caller->BuildRotation = ViewRotation.Yaw;
		
		FString Msg = FString::Printf(TEXT("Roll:%f Pitch:%f Yaw:%f R:%f Distance:%f X:%f, Y:%f"), ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw, Caller->BuildRotation, Caller->BuildDistance,Caller->BuildLocation.X,Caller->BuildLocation.Y);
		//LogScreen(1.0f,Msg);

		//检查是否有物体可以附着
		if(IsAttach)
		{
			FString BlockName = BlockActorName;
			FVector BlockActorLocation = FVector::DownVector;
			FRotator BlockActorRotation = FRotator::ZeroRotator;
			for(int32 i = 0; i < Caller->SavingCache.Num(); ++i)
			{
				if(BlockName == Caller->SavingCache[i].Building.GetName())
				{
					BlockActorLocation = Caller->SavingCache[i].Location;
					BlockActorRotation = Caller->SavingCache[i].Rotation;
					Caller->Index = i;
					break;
				}
			}

			if(BlockActorLocation != FVector::DownVector && (Caller->MainCharacter->GetVelocity() == FVector::ZeroVector && FMath::Sqrt(FMath::Square(Caller->BuildLocation.X - BlockActorLocation.X) + FMath::Square(Caller->BuildLocation.Y - BlockActorLocation.Y)) < 250.0f))
			{
				float Side = Caller->BuildItem->HalfSizeX * 2;
				FString AttachSide = Caller->BuildItem-> BlockActorSide;
				
				Caller->BuildItem->ForceBuild = true;
				if(AttachSide.Contains("Right") && !Caller->SavingCache[Caller->Index].Right)
				{
					Caller->BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
					Caller->BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
					Caller->BuildLocation.Z = BlockActorLocation.Z;
					Caller->WhichSide = "Right";
					Caller->BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Left") && !Caller->SavingCache[Caller->Index].Left)
				{
					Caller->BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					Caller->BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					Caller->BuildLocation.Z = BlockActorLocation.Z;
					Caller->WhichSide = "Left";
					Caller->BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Up") && !Caller->SavingCache[Caller->Index].Up)
				{
					Caller->BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					Caller->BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					Caller->BuildLocation.Z = BlockActorLocation.Z;
					Caller->WhichSide = "Up";
					Caller->BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Down") && !Caller->SavingCache[Caller->Index].Down)
				{
					Caller->BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					Caller->BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					Caller->BuildLocation.Z = BlockActorLocation.Z;
					Caller->WhichSide = "Down";
					Caller->BuildRotation = BlockActorRotation.Yaw;
				}
				else
				{
					Caller->BuildItem->ForceBuild = false;
				}
				if(Caller->BuildItem->ForceBuild && IsBlock)
				{
					SetBlur(0.1,FColor::Green);
				}

				FString LocationMsg = FString::Printf(TEXT("X:%f Y:%f X:%f Y:%f WhichSide:%s"), BlockActorLocation.X,BlockActorLocation.Y,Caller->BuildLocation.X,Caller->BuildLocation.Y,*Caller->WhichSide);
                //LogScreen(1.0f,LocationMsg);
			}
		}
		
		Caller->BuildItem->SetActorRotation(FRotator(0,Caller->BuildRotation,0));
		Caller->BuildItem->SetActorLocation(Caller->BuildLocation);
}
