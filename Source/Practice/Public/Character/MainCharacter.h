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

	void SetMouthType(const FString& MouthType);
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
	TArray<FString> IdleActionArray;
	
protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	virtual void Jump() override;

	virtual void StopJumping() override;
	
	void Run();
	
	void StopRunning();

	void Zoom(const FInputActionValue& Value);
	
	void FirstPerson();
	
	void ThirdPerson();

	UPROPERTY(Category=Camera, EditAnywhere)
	bool bUseFirstPerson = false;
	
	//蓝图中实现 未使用
	void PlayAnim(FString Value, bool Loop = false);

	void SwitchAnim();
	
	void PlayIdleAction();
	
	void PlayDefaultIdle();
	
	FString GetPlayingAnimName();
	
	FTimerHandle IdleActionTimerHandle;
	FTimerHandle IdleTimerHandle;

	bool IsIdleCouldPlay = true;
	bool IsWalkCouldPlay = true;
	bool IsJumpCouldPlay = true;
	bool IsIdleActionCouldPlay = false;

	UPROPERTY(EditDefaultsOnly,Category="Material")
	FString DefaultMouthType = "Mouth_0_4";
	
	UPROPERTY(EditDefaultsOnly,Category="Material")
	FString RunMouthType = "Mouth_1_2";
	
	UPROPERTY(EditDefaultsOnly,Category="Material")
	FString JumpMouthType = "Mouth_0_3";
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
	TObjectPtr<UInputAction> RunAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ViewAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anime, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> AnimSequence;
};
