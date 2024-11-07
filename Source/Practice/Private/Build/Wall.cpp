// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/Wall.h"

#include "Build/BuildSystem.h"
#include "Character/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *WallMesh));

	BoxComponent->InitBoxExtent(FVector(HalfSizeX, HalfSizeY-HalfSizeY/10, HalfSizeZ-HalfSizeZ/10));
	
	UpSideBoxComponent->InitBoxExtent(FVector(HalfSizeX*2, HalfSizeY/2, HalfSizeZ/10));
	UpSideBoxComponent->SetRelativeLocation(FVector(0, 0, HalfSizeZ));
	
	DownSideBoxComponent->InitBoxExtent(FVector(HalfSizeX*2, HalfSizeY/2, HalfSizeZ/10));
	DownSideBoxComponent->SetRelativeLocation(FVector(0, 0, -HalfSizeZ));
	
	LeftSideBoxComponent->InitBoxExtent(FVector(HalfSizeX*2, HalfSizeY/10, HalfSizeZ/2));
	LeftSideBoxComponent->SetRelativeLocation(FVector(0, -HalfSizeY, 0));
	
	RightSideBoxComponent->InitBoxExtent(FVector(HalfSizeX*2, HalfSizeY/10, HalfSizeZ/2));
	RightSideBoxComponent->SetRelativeLocation(FVector(0, HalfSizeY, 0));
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

void AWall::OnBlurAttach(UBuildSystem* Caller)
{
		FRotator ViewRotation = Caller->MainCharacter->GetController()->GetControlRotation();
		FVector MainLocation = Caller->MainCharacter->GetActorLocation();
		float OriginAngle = ViewRotation.Pitch;
		float AngleControllerPlayer = (ViewRotation.Pitch > 270  && ViewRotation.Pitch < 360) ? ViewRotation.Pitch - 270 : 89;
		/*
		 *
		 *
		 *         ↗
		 *camera ↗
		 *       ↘-------------------------------
		 *     z p ↘  AngleControllerPlayer
		 *       p θ ↘
		 *       p     ↘
		 *       p      ↘
		 *       --------- distance = z*tanθ
		 */
		Caller->BuildDistance = FMath::Clamp(FMath::Tan(FMath::DegreesToRadians(AngleControllerPlayer)) * MainLocation.Z,100.0f,300.0f);
		LogScreen(1,FString::Printf(TEXT("%f %f"),OriginAngle,AngleControllerPlayer));
		float X = Caller->BuildDistance * FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw));
		float Y = Caller->BuildDistance * FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw));
		float Z = 0;
		if(OriginAngle > 0 && OriginAngle < 90)
		{
			Z = FMath::Clamp(Caller->BuildDistance * FMath::Tan(FMath::DegreesToRadians(OriginAngle)),0,HalfSizeZ*2);
			X = (301.0f - Z) * FMath::Cos(FMath::DegreesToRadians(ViewRotation.Yaw));
			Y = (301.0f - Z) * FMath::Sin(FMath::DegreesToRadians(ViewRotation.Yaw));
		}
		Caller->BuildLocation = FVector(MainLocation.X,MainLocation.Y,HalfSizeZ) + FVector(X,Y,Z);
		Caller->BuildRotation = ViewRotation.Yaw;
		
		FString Msg = FString::Printf(TEXT("Roll:%f Pitch:%f Yaw:%f R:%f Distance:%f X:%f, Y:%f"), ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw, Caller->BuildRotation, Caller->BuildDistance,Caller->BuildLocation.X,Caller->BuildLocation.Y);
		//LogScreen(1.0f,Msg);

		//检查是否有物体可以附着
		FString BlockName = BlockActorName;
		if(IsAttach)
		{
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
			
				ForceBuild = true;
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
					ForceBuild = false;
				}
				FString LocationMsg = FString::Printf(TEXT("X:%f Y:%f X:%f Y:%f WhichSide:%s"), BlockActorLocation.X,BlockActorLocation.Y,Caller->BuildLocation.X,Caller->BuildLocation.Y,*Caller->WhichSide);
                //LogScreen(1.0f,LocationMsg);
			}
		}
		
		Caller->BuildItem->SetActorRotation(FRotator(0,Caller->BuildRotation,0));
		Caller->BuildItem->SetActorLocation(Caller->BuildLocation);
}


