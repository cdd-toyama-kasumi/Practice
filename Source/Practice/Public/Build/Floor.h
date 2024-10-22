// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

class UBoxComponent;

UCLASS()
class PRACTICE_API AFloor : public AActor
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
	void SetCollision(ECollisionEnabled::Type Type);

	UFUNCTION()
	void SetBlur(float Emissive,FColor Color);
	
	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category="Collision")
	FString BlockActorName;
	
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeXY = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeZ = 3.f;

	bool IsBlock = false;

	bool ForceBuild = false;
	
	void SetMaterial(FString Material);
	
	UFUNCTION()
	void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
