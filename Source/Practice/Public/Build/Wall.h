// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildBase.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

class UBoxComponent;

UCLASS()
class PRACTICE_API AWall : public ABuildBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	virtual void OnBlurAttach(UBuildSystem* Caller) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
