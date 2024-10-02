// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildSystem.generated.h"


class AMainCharacter;

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

	UPROPERTY(EditDefaultsOnly, Category="Base")
	TObjectPtr<AMainCharacter> MainCharacter;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	float BuildDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category="Base")
	FVector BuildLocation;

	UPROPERTY(EditInstanceOnly, Category="Base")
	TObjectPtr<UObject> BuildItem = nullptr;
	
	void SetPlayer(AMainCharacter* Player);

	void SetBuildItem();

	void UnSetBuildItem();

	bool Building();
};
