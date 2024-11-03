// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildBase.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

class UBoxComponent;



UCLASS()
class PRACTICE_API AFloor : public ABuildBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
