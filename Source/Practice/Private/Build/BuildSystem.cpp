// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/BuildSystem.h"

#include "IDetailTreeNode.h"
#include "InputActionValue.h"
#include "Build/Floor.h"
#include "Build/Wall.h"
#include "Character/MainCharacter.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

void FBuildType::Pre()
{
	if(CurIndex != 0)
	{
		--CurIndex;
	}
	else
	{
		CurIndex = BuildingTypeArray.Num()-1;
	}
}

void FBuildType::Next()
{
	if(CurIndex != BuildingTypeArray.Num()-1)
	{
		++CurIndex;
	}
	else
	{
		CurIndex = 0;
	}
}

EBuildingType FBuildType::GetType()
{
	return BuildingTypeArray[CurIndex];
}

bool FBuildType::operator==(const EBuildingType& Type)
{
	return BuildingTypeArray[CurIndex] == Type;
}

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
	if(BuildItem)
	{
		UnSetBuildItem();
	}

	if(CurType == EBuildingType::Floor)
	{
		BuildItem = GetWorld()->SpawnActor<AFloor>(FVector(0,0,-10000),FRotator::ZeroRotator);
	}
	else if(CurType == EBuildingType::Wall)
	{
		BuildItem = GetWorld()->SpawnActor<AWall>(FVector(0,0,-10000),FRotator::ZeroRotator);
	}
	BuildItem->SetBlur(0.1,FColor::Green);
	BuildItem->SetCollision(ECollisionEnabled::QueryOnly);
}

void UBuildSystem::UnSetBuildItem()
{
	if(BuildItem)
	{
		GetWorld()->DestroyActor(BuildItem);
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
		BuildRotation = ViewRotation.Yaw;
		
		FString Msg = FString::Printf(TEXT("Roll:%f Pitch:%f Yaw:%f R:%f Distance:%f X:%f, Y:%f"), ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw, BuildRotation, BuildDistance,BuildLocation.X,BuildLocation.Y);
		//LogScreen(1.0f,Msg);

		//检查是否有物体可以附着
		FString BlockName = BuildItem->BlockActorName;
		if(BuildItem->IsAttach)
		{
			FVector BlockActorLocation = FVector::DownVector;
			FRotator BlockActorRotation = FRotator::ZeroRotator;
			for(int32 i = 0; i < SavingCache.Num(); ++i)
			{
				if(BlockName == SavingCache[i].Building.GetName())
				{
					BlockActorLocation = SavingCache[i].Location;
					BlockActorRotation = SavingCache[i].Rotation;
					Index = i;
					break;
				}
			}

			if(BlockActorLocation != FVector::DownVector && (MainCharacter->GetVelocity() == FVector::ZeroVector && FMath::Sqrt(FMath::Square(BuildLocation.X - BlockActorLocation.X) + FMath::Square(BuildLocation.Y - BlockActorLocation.Y)) < 200.0f))
			{
				float Side = BuildItem->HalfSizeX * 2;
				FString AttachSide = BuildItem-> BlockActorSide;
			
				ForceBuild = true;
				BuildItem->ForceBuild = true;
				if(AttachSide.Contains("Right") && !SavingCache[Index].Right)
				{
					BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
					BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw)) * Side;
					BuildLocation.Z = BlockActorLocation.Z;
					WhichSide = "Right";
					BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Left") && !SavingCache[Index].Left)
				{
					BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 180)) * Side;
					BuildLocation.Z = BlockActorLocation.Z;
					WhichSide = "Left";
					BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Up") && !SavingCache[Index].Up)
				{
					BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 270)) * Side;
					BuildLocation.Z = BlockActorLocation.Z;
					WhichSide = "Up";
					BuildRotation = BlockActorRotation.Yaw;
				}
				else if(AttachSide.Contains("Down") && !SavingCache[Index].Down)
				{
					BuildLocation.X = BlockActorLocation.X + FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					BuildLocation.Y = BlockActorLocation.Y + FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw + 90)) * Side;
					BuildLocation.Z = BlockActorLocation.Z;
					WhichSide = "Down";
					BuildRotation = BlockActorRotation.Yaw;
				}
				else
				{
					BuildItem->ForceBuild = false;
					ForceBuild = false;
				}
				FString LocationMsg = FString::Printf(TEXT("X:%f Y:%f X:%f Y:%f WhichSide:%s"), BlockActorLocation.X,BlockActorLocation.Y,BuildLocation.X,BuildLocation.Y,*WhichSide);
                //LogScreen(1.0f,LocationMsg);
			}
		}
		
		BuildItem->SetActorRotation(FRotator(0,BuildRotation,0));
		BuildItem->SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::Building()
{
	if(!BuildItem)
	{
		return false;
	}
	if(BuildItem->IsBlock && !ForceBuild)
	{
		LogScreen(1.0f,FString::Printf(TEXT("%ls:Building is blocked"), *BuildItem->GetName()));
		return false;
	}
	
	BuildItem->MeshComponent->SetMobility(EComponentMobility::Stationary);
	BuildItem->SetCollision(ECollisionEnabled::QueryAndPhysics);
	BuildItem->SetBlur(0.0,FColor::Black);
	BuildItem->IsSet = true;

	FBuildCache Cache;
	Cache.Type = "floor";
	Cache.Location = BuildLocation;
	Cache.Rotation = BuildItem->GetActorRotation();
	Cache.Building = BuildItem;
	
	if(BuildItem->IsAttach)
	{
		for(int32 i = 0; i < SavingCache.Num(); ++i)
		{
			for(FAttachActor Item : BuildItem->AttachActorCache)
			{
				if(Item.Name == SavingCache[i].Building.GetName())
				{
					if(Item.Side.Contains("Right"))
					{
						SavingCache[i].Right = true;
					}
					else if(Item.Side.Contains("Left"))
					{
						SavingCache[i].Left = true;
					}
					else if(Item.Side.Contains("Up"))
					{
						SavingCache[i].Up = true;
					}
					else if(Item.Side.Contains("Down"))
					{
						SavingCache[i].Down = true;
					}
					break;
				}
			}
			Cache.Right = BuildItem->ArraySide[static_cast<int>(Direction::Right)];
			Cache.Left = BuildItem->ArraySide[static_cast<int>(Direction::Left)];
			Cache.Up = BuildItem->ArraySide[static_cast<int>(Direction::Up)];
			Cache.Down = BuildItem->ArraySide[static_cast<int>(Direction::Down)];
		}
	}
	
	SavingCache.Emplace(Cache);
	BuildItem = nullptr;
	return true;
}

void UBuildSystem::SwitchType(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	if(AxisValue < 0)
	{
		CurType.Pre();
	}
	else
	{
		CurType.Next();
	}
	SetBuildItem();
}