// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "BuildSystem.generated.h"

class AMainCharacter;
class ABuildBase;

USTRUCT()
struct FBuildCache
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly,Category="base")
	FVector Location;
	
	UPROPERTY(EditInstanceOnly,Category="base")
	FRotator Rotation;

	UPROPERTY(EditInstanceOnly,Category="base")
	FString Type;
	
	UPROPERTY(EditInstanceOnly,Category="base")
	bool Up = false;

	UPROPERTY(EditInstanceOnly,Category="base")
	bool Down = false;

	UPROPERTY(EditInstanceOnly,Category="base")
	bool Left = false;

	UPROPERTY(EditInstanceOnly,Category="base")
	bool Right = false;

	UPROPERTY(EditInstanceOnly,Category="base")
	TObjectPtr<UObject> Building = nullptr;
};

enum EBuildingType
{
	Floor,
	Wall
};


class FBuildType
{
public:

	void Pre();
	void Next();
	
	EBuildingType GetType();

	bool operator==(const EBuildingType& Type);
	
	TArray<EBuildingType> BuildingTypeArray{Floor, Wall};
	int32 CurIndex = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_API UBuildSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetPlayer(AMainCharacter* Player);

	void SetBuildItem();

	void UnSetBuildItem();

	void BlurAttach();

	bool Building();

	void SwitchType(const FInputActionValue& Value);
public:
	UPROPERTY(EditDefaultsOnly, Category="Base")
	TObjectPtr<AMainCharacter> MainCharacter;

	UPROPERTY(EditInstanceOnly, Category="Base")
	TObjectPtr<ABuildBase> BuildItem = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Base")
	float BuildDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	FVector BuildLocation;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	float BuildRotation;
	
	UPROPERTY(EditInstanceOnly, Category="Base")
	TArray<FBuildCache> SavingCache;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	bool ForceBuild = true;
	
	UPROPERTY(VisibleAnywhere, Category="Base")
	int32 Index = 0;

	UPROPERTY(VisibleAnywhere, Category="Base")
	FString WhichSide;

	FBuildType CurType;
};
