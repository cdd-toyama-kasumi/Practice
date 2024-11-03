// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildBase.generated.h"

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
class PRACTICE_API ABuildBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetCollision(ECollisionEnabled::Type Type);

	UFUNCTION()
	void SetBlur(float Emissive,FColor Color);
	
	void SetMaterial(const FString& Material);
	
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
	
public://
	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attach")
	TObjectPtr<UBoxComponent> RightSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attach")
	TObjectPtr<UBoxComponent> LeftSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attach")
	TObjectPtr<UBoxComponent> UpSideBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attach")
	TObjectPtr<UBoxComponent> DownSideBoxComponent;

public:
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeX = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeY = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="BaseParam")
	float HalfSizeZ = 150.f;

	//吸附的物体信息
	UPROPERTY(EditInstanceOnly, Category="Collision")
	TArray<FAttachActor> AttachActorCache;

	//吸附对象名称
	UPROPERTY(VisibleAnywhere, Category="Attach")
	FString BlockActorName;

	//吸附边名称
	UPROPERTY(VisibleAnywhere, Category="Attach")
	FString BlockActorSide;
	
	//是否碰撞
	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsBlock = false;

	UPROPERTY(EditInstanceOnly, Category="Base")
	bool ForceBuild = false;

	//是否被放置
	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsSet = false;

	//当前是否吸附
	UPROPERTY(EditInstanceOnly, Category="Base")
	bool IsAttach = false;

	//边吸附状态
	UPROPERTY(EditInstanceOnly, Category="Base")
	TArray<bool> ArraySide{false,false,false,false};
};
