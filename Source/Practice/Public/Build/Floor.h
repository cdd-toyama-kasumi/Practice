// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

class UBoxComponent;

USTRUCT()
struct FAttachActor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, Category="Base")
	FString Name;
	
	UPROPERTY(EditInstanceOnly, Category="Base")
	FString Side;
};

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

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> RightSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> LeftSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> UpSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> DownSideBoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Collision")
	FString BlockActorName;

	UPROPERTY(VisibleAnywhere, Category="Collision")
	FString BlockActorSide;

	UPROPERTY(EditInstanceOnly, Category="Collision")
	TArray<FAttachActor> AttachActorCache;
	
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeXY = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeZ = 3.f;

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

	
	void SetMaterial(FString Material);
	
	UFUNCTION()
	void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void RightSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void RightSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void LeftSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void LeftSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void UpSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void UpSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DownSideBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void DownSideEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	bool Save(FString Name, FString CompName);

	bool Remove(FString Name, FString CompName);
};
