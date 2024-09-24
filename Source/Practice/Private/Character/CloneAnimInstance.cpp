// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CloneAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UCloneAnimInstance::UCloneAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
}

void UCloneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MainCharacter = Cast<AMainCharacter>(GetOwningActor());
	if(MainCharacter)
	{
		CharacterMovementComponent = MainCharacter->GetCharacterMovement();
	}
	
}

void UCloneAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(!MainCharacter)
	{
		return;
	}

	const FVector Velocity = MainCharacter->GetVelocity();
	Speed = Velocity.Size();
	IsFalling = CharacterMovementComponent->IsFalling();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, MainCharacter->GetActorRotation());
}
