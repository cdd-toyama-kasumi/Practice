// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CloneAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tools/Log.h"

UCloneAnimInstance::UCloneAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
	IdleActionTime = 10.0f;
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
	if(!MainCharacter || !CharacterMovementComponent)
	{
		return;
	}
	
	const FVector Velocity = MainCharacter->GetVelocity();
	Speed = Velocity.Size();
	IsFalling = CharacterMovementComponent->IsFalling();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, MainCharacter->GetActorRotation());

	if(Speed > 0.0f)
	{
		IsIdleActionCouldPlay = false;
		MainCharacter->GetWorldTimerManager().ClearTimer(IdleActionTimerHandle);
	}
	else
	{
		if(!IdleActionTimerHandle.IsValid())
		{
			MainCharacter->GetWorldTimerManager().SetTimer(IdleActionTimerHandle,this,&UCloneAnimInstance::PlayIdleAction,IdleActionTime,true);
		}
	}
}

void UCloneAnimInstance::PlayDefaultIdle()
{
	LogScreenRed(5, "PlayDefaultIdle");
	IsIdleActionCouldPlay = false;
	MainCharacter->GetWorldTimerManager().ClearTimer(IdleTimerHandle);
	MainCharacter->SetMouthType("Mouth_0_4");
}

void UCloneAnimInstance::PlayIdleAction()
{
	LogScreenRed(5, "PlayIdleAction");
	IsIdleActionCouldPlay = true;
	MainCharacter->GetWorldTimerManager().SetTimer(IdleTimerHandle,this,&UCloneAnimInstance::PlayDefaultIdle,2.0f,true);
	MainCharacter->SetMouthType("Mouth_1_4");
}