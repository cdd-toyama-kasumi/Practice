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
		BuildItem->OnBlurAttach(this);
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
	switch(CurType.GetType())
	{
	case EBuildingType::Floor:
		Cache.Type = "floor";
	case EBuildingType::Wall:
		Cache.Type = "wall";
	default:
		break;
	}
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
