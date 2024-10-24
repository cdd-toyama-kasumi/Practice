// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CloneAnimInstance.generated.h"

class UCharacterMovementComponent;
class AMainCharacter;
/**
 * 
 */
UCLASS()
class PRACTICE_API UCloneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCloneAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayDefaultIdle();
	void PlayIdleAction();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<AMainCharacter> MainCharacter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Direction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsFalling = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsIdleActionCouldPlay = false;

	FTimerHandle IdleActionTimerHandle;
	FTimerHandle IdleTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	int32 IdleActionTime;
};
