// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PRACTICE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	//第一第三人称切换
	void SwitchPerspective();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Construct();
	void GenerateMainCamera();
	void GenerateMainBody();

private:
	FString Idle = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idle_Anim.idle_Anim'");
	FString Walk = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/walk_Anim.walk_Anim'");
	FString JumpAnim = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/action_Anim.action_Anim'");
	FString JumpLoopAnim = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/action_Anim.action_Anim'");
	FString IdleActionAnim1 = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idleAction_Anim1.idleAction_Anim1'");
	FString IdleActionAnim2 = TEXT("AnimSequence'/Game/Mine/Airi/AnimSequence/idleAction_Anim2.idleAction_Anim2'");
	
	TArray<FString> IdleActionArray;
protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void Jump(const FInputActionValue& Value);

	void StopJumping(const FInputActionValue& Value);
	
	void Zoom(const FInputActionValue& Value);
	
	void FirstPerson();
	
	void ThirdPerson();
	
	void PlayAnim(FString Value, bool Loop = false);

	void SwitchAnim();
	
	void PlayIdleAction();
	void PlayDefaultIdle();
	
	FString GetPlayingAnimName();
	
	FTimerHandle IdleActionTimerHandle;
	FTimerHandle IdleTimerHandle;
	UPROPERTY(Category=Camera, EditAnywhere)
	bool bUseFirstPerson = false;
	
	UPROPERTY(Category=Input, VisibleAnywhere)
	bool IsMove = false;

	bool IsIdleCouldPlay = true;
	bool IsWalkCouldPlay = true;
	bool IsJumpCouldPlay = true;
	bool IsIdleActionCouldPlay = false;
	
private:
	UPROPERTY(Category=Character, EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponentHalo;
	
	UPROPERTY(Category=Character, EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponentHalo;
	
	UPROPERTY(Category=Character, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMesh> SkeletalMeshBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ViewAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anime, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> AnimSequence;
	
};
