// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/BuildSystem.h"

#include "IDetailTreeNode.h"
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
		BuildRotation = ViewRotation.Yaw;
		
		FString Msg = FString::Printf(TEXT("Roll:%f Pitch:%f Yaw:%f Distance:%f X:%f, Y:%f"), ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw,BuildDistance,BuildLocation.X,BuildLocation.Y);
		//LogScreen(1.0f,Msg);

		//检查是否有物体可以附着
		FString BlockName = Cast<AFloor>(BuildItem)->BlockActorName;
		if(Cast<AFloor>(BuildItem)->IsAttach)
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
			
			if(BlockActorLocation != FVector::DownVector && (MainCharacter->GetVelocity() == FVector::ZeroVector))
			{
				float Side = Cast<AFloor>(BuildItem)->HalfSizeXY * 2;
				FString AttachSide = Cast<AFloor>(BuildItem)-> BlockActorSide;
				/*float CenterToVertex = Cast<AFloor>(BuildItem)->HalfSizeXY * FMath::Sqrt(2.0f);
				//右上角坐标
				float RightUpX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw+315)) * CenterToVertex;
				float RightUpY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw+315)) * CenterToVertex;
				//左上角坐标
				float LeftUpX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw+225)) * CenterToVertex;
				float LeftUpY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw+255)) * CenterToVertex;
				//左下角坐标
				float LeftDownX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw+135)) * CenterToVertex;
				float LeftDownY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw+135)) * CenterToVertex;
				//右下角坐标
				float RightDownX = FMath::Cos(FMath::DegreesToRadians(BlockActorRotation.Yaw+45)) * CenterToVertex;
				float RightDownY = FMath::Sin(FMath::DegreesToRadians(BlockActorRotation.Yaw+45)) * CenterToVertex;
				
				FVector RightSideCenter = BlockActorLocation + FVector((RightUpX + RightDownX)/2,(RightUpY + RightDownY)/2,0);
				FVector LeftSideCenter = BlockActorLocation + FVector((LeftUpX + LeftDownX)/2,(LeftUpY + LeftDownY)/2,0);
				FVector UpSideCenter = BlockActorLocation + FVector((RightUpX + LeftUpX)/2,(RightUpY + LeftUpY)/2,0);
				FVector DownSideCenter = BlockActorLocation + FVector((RightDownX + LeftDownX)/2,(RightDownY + LeftDownY)/2,0);
				
				float DistanceToRight = FMath::Sqrt(FMath::Square(BuildLocation.X-RightSideCenter.X) + FMath::Square(BuildLocation.Y-RightSideCenter.Y));
				float DistanceToLeft = FMath::Sqrt(FMath::Square(BuildLocation.X-LeftSideCenter.X) + FMath::Square(BuildLocation.Y-LeftSideCenter.Y));
				float DistanceToUp = FMath::Sqrt(FMath::Square(BuildLocation.X-UpSideCenter.X) + FMath::Square(BuildLocation.Y-UpSideCenter.Y));
				float DistanceToDown = FMath::Sqrt(FMath::Square(BuildLocation.X-DownSideCenter.X) + FMath::Square(BuildLocation.Y-DownSideCenter.Y));

				FString DistanceMsg = FString::Printf(TEXT("DisRight:%f DisToLeft:%f DisUp:%f DisDown:%f RightUpX:%f RightUpY:%f LeftUpX:%f LeftUpY:%f LeftDownX:%f LeftDownY:%f RightDownX:%f RightDownY:%f\nRightX:%f RightY:%f UpX:%f UpY:%f LeftX:%f LeftY:%f DownX:%f DownY:%f"), DistanceToRight,DistanceToLeft,DistanceToUp,DistanceToDown,RightUpX,RightUpY,
				LeftUpX,LeftUpY,LeftDownX,LeftDownY,RightDownX,RightDownY,RightSideCenter.X,RightSideCenter.Y,
									UpSideCenter.X,UpSideCenter.Y,LeftSideCenter.X,LeftSideCenter.Y,DownSideCenter.X,DownSideCenter.Y);
				//LogScreen(1.0f,DistanceMsg);
				
				float MinDis = FMath::Min(DistanceToRight,FMath::Min(DistanceToLeft,FMath::Min(DistanceToUp,DistanceToDown)));*/
				
				ForceBuild = true;
				Cast<AFloor>(BuildItem)->ForceBuild = true;
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
					Cast<AFloor>(BuildItem)->ForceBuild = false;
					ForceBuild = false;
				}
				FString LocationMsg = FString::Printf(TEXT("X:%f Y:%f X:%f Y:%f WhichSide:%s"), BlockActorLocation.X,BlockActorLocation.Y,BuildLocation.X,BuildLocation.Y,*WhichSide);
                //LogScreen(1.0f,LocationMsg);
			}
		}
		
		Cast<AFloor>(BuildItem)->SetActorRotation(FRotator(0,BuildRotation,0));
		Cast<AFloor>(BuildItem)->SetActorLocation(BuildLocation);
	}
}

bool UBuildSystem::Building()
{
	if(!BuildItem)
	{
		return false;
	}
	if(Cast<AFloor>(BuildItem)->IsBlock && !ForceBuild)
	{
		LogScreen(1.0f,FString::Printf(TEXT("%ls:Building is blocked"), *BuildItem->GetName()));
		return false;
	}
	
	Cast<AFloor>(BuildItem)->MeshComponent->SetMobility(EComponentMobility::Stationary);
	Cast<AFloor>(BuildItem)->SetCollision(ECollisionEnabled::QueryAndPhysics);
	Cast<AFloor>(BuildItem)->SetBlur(0.0,FColor::Black);
	Cast<AFloor>(BuildItem)->IsSet = true;

	FBuildCache Cache;
	Cache.Type = "floor";
	Cache.Location = BuildLocation;
	Cache.Rotation = Cast<AFloor>(BuildItem)->GetActorRotation();
	Cache.Building = BuildItem;
	
	if(Cast<AFloor>(BuildItem)->IsAttach)
	{
		for(int32 i = 0; i < SavingCache.Num(); ++i)
		{
			for(FAttachActor Item : Cast<AFloor>(BuildItem)->AttachActorCache)
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
			Cache.Right = Cast<AFloor>(BuildItem)->ArraySide[static_cast<int>(Direction::Right)];
			Cache.Left = Cast<AFloor>(BuildItem)->ArraySide[static_cast<int>(Direction::Left)];
			Cache.Up = Cast<AFloor>(BuildItem)->ArraySide[static_cast<int>(Direction::Up)];
			Cache.Down = Cast<AFloor>(BuildItem)->ArraySide[static_cast<int>(Direction::Down)];
		}
			
		/*
		if(ForceBuild)
		{
			if(WhichSide == "Right")
			{
				SavingCache[Index].Right = true;
			}
			else if(WhichSide == "Left")
			{
				SavingCache[Index].Left = true;
			}
			else if(WhichSide == "Up")
			{
				SavingCache[Index].Up = true;
			}
			else if(WhichSide == "Down")
			{
				SavingCache[Index].Down = true;
			}
		}
		*/

	}
	
	SavingCache.Emplace(Cache);
	BuildItem = nullptr;
	return true;
}

