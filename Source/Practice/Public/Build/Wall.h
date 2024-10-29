// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

class UBoxComponent;

UCLASS()
class PRACTICE_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> RightSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> LeftSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> UpSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> DownSideBoxComponent;
	
	//吸附对象名称
	UPROPERTY(VisibleAnywhere, Category="Collision")
	FString BlockActorName;

	//吸附边名称
	UPROPERTY(VisibleAnywhere, Category="Collision")
	FString BlockActorSide;

	/*//吸附的物体信息
	UPROPERTY(EditInstanceOnly, Category="Collision")
	TArray<FAttachActor> AttachActorCache;*/
	
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeX = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeY = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeZ = 150.f;

	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsBlock = false;

	UPROPERTY(EditInstanceOnly, Category="Base")
	bool ForceBuild = false;

	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsSet = false;

	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsAttach = false;

	UPROPERTY(EditInstanceOnly, Category="Base")
	TArray<bool> ArraySide{false,false,false,false};
};
