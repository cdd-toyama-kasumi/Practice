// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MainCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tools/Log.h"
#include "Tools/Static.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Construct();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(bUseFirstPerson)
	{
		FirstPerson();
	}
	else
	{
		ThirdPerson();
	}
	if(UClass* AnimationClass = LoadObject<UClass>(nullptr,*AnimInstance))
	{
		GetMesh()->SetAnimInstanceClass(AnimationClass);
	}
	//PlayAnim(Idle,true);
}

void AMainCharacter::Construct()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(54.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	
	IdleActionArray = {IdleActionAnim1,IdleActionAnim2};
	
	GenerateMainBody();
	GenerateMainCamera();

	GetCharacterMovement()->MaxWalkSpeed = 120.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 50.0f;
}

void AMainCharacter::GenerateMainBody()
{
	MeshComponentHalo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponentHalo"));
	MeshComponentHalo->CastShadow = false;//光环无实体 不产生阴影
	MeshComponentHalo->SetCollisionProfileName(TEXT("NoCollision"));
	
	SpringArmComponentHalo = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponentHalo"));
	SpringArmComponentHalo->TargetArmLength = 60.0f;
	SpringArmComponentHalo->SetupAttachment(GetCapsuleComponent());
	SpringArmComponentHalo->SetRelativeRotation(FRotator(-60.0f,0.0f,0.0f));
	SpringArmComponentHalo->bDoCollisionTest = false;
	MeshComponentHalo->SetupAttachment(SpringArmComponentHalo,TEXT("SpringEndpoint"));
	//设置光环的延迟移动
	SpringArmComponentHalo->bEnableCameraLag = true;
	SpringArmComponentHalo->bEnableCameraRotationLag = true;
	SpringArmComponentHalo->CameraLagSpeed = 7.0f;
	SpringArmComponentHalo->CameraRotationLagSpeed = 7.0f;
	SpringArmComponentHalo->CameraLagMaxDistance = 14.0f;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HaloMesh(*Halo);
	SkeletalMeshBody = LoadObject<USkeletalMesh>(nullptr,*SkeletalMesh);
	
	GetMesh()->SetRelativeLocation((FVector(0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight())));
	GetMesh()->SetSkeletalMesh(SkeletalMeshBody);
}

void AMainCharacter::GenerateMainCamera()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, TEXT("SpringEndpoint"));
	ThirdPerson();
}

void AMainCharacter::SwitchPerspective()
{
	bUseFirstPerson = !bUseFirstPerson;
	if(bUseFirstPerson)
	{
		FirstPerson();
	}
	else
	{
		ThirdPerson();
	}
}

bool AMainCharacter::IsRun()
{
	return IsRunning;
}

void AMainCharacter::FirstPerson()
{
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->SetRelativeLocation(FVector(20.0f,0.0f,20.0f));
	bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;
    bUseControllerRotationPitch = false;
}

void AMainCharacter::ThirdPerson()
{
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainCharacter::Zoom(const FInputActionValue& Value)
{
	if(bUseFirstPerson) return;
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength + Value.Get<float>() * 50.0f,50.0f,300.0f);
}

void AMainCharacter::PlayAnim(FString Value, bool Loop)
{
	AnimSequence = LoadObject<UAnimSequence>(nullptr,*Value);
	if(AnimSequence)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAnimation(AnimSequence);
		GetMesh()->Play(Loop);
	}
}

void AMainCharacter::SwitchAnim()
{
	bool IsFalling = GetCharacterMovement()->IsFalling();
	bool IsCrouching = GetCharacterMovement()->IsCrouching();
	bool IsPlaying = GetMesh()->IsPlaying();
	bool IsJumpStart = GetPlayingAnimName() == JumpAnim;

	FString Status = FString::Printf(TEXT("IsFalling:%s IsCrouching:%s IsPlaying:%s IsJumpStart:%s IsJumpCouldPlay:%s IsWalkCouldPlay:%s IsIdleCouldPlay:%s IsIdleActionCouldPlay:%s "),
		IsFalling ? TEXT("True") : TEXT("False"),
		IsCrouching ? TEXT("True") : TEXT("False"),
		IsPlaying ? TEXT("True") : TEXT("False"),
		IsJumpStart ? TEXT("True") : TEXT("False"),
		IsJumpCouldPlay ? TEXT("True") : TEXT("False"),
		IsWalkCouldPlay ? TEXT("True") : TEXT("False"),
		IsIdleCouldPlay ? TEXT("True") : TEXT("False"),
		IsIdleActionCouldPlay ? TEXT("True") : TEXT("False")) + GetPlayingAnimName();
	
	LogScreenRaw(0.5f,Status);
	
	if(IsFalling && IsJumpCouldPlay)
	{
		IsJumpCouldPlay = false;
		IsWalkCouldPlay = true;
		IsIdleCouldPlay = true;
		IsIdleActionCouldPlay = false;
		if(IsJumpStart && !IsPlaying)
		{
			PlayAnim(JumpLoopAnim);
		}
	}
	else if(UKismetMathLibrary::VSize(GetVelocity()) > 0.0f)
	{
		if(IsWalkCouldPlay && !IsFalling)
		{
			IsJumpCouldPlay = true;
			IsWalkCouldPlay = false;
			IsIdleCouldPlay = true;
			IsIdleActionCouldPlay = false;
			PlayAnim(Walk,true);
		}
	}
	else if(IsIdleCouldPlay && !IsFalling)
	{
		IsJumpCouldPlay = true;
		IsWalkCouldPlay = true;
		IsIdleCouldPlay = false;
		PlayAnim(Idle,true);
		GetWorldTimerManager().SetTimer(IdleActionTimerHandle,this,&AMainCharacter::PlayIdleAction,6.0f,true);
		GetWorldTimerManager().ClearTimer(IdleTimerHandle);
	}
	else
	{
		if(IsIdleActionCouldPlay && !IsFalling)
		{
			IsJumpCouldPlay = true;
			IsWalkCouldPlay = true;
			IsIdleActionCouldPlay = false;
			int32_t RandIndex = FMath::RandRange(0,IdleActionArray.Num()-1);
			PlayAnim(IdleActionArray[RandIndex],false);
			GetWorldTimerManager().SetTimer(IdleTimerHandle,this,&AMainCharacter::PlayDefaultIdle,AnimSequence->GetPlayLength(),true);
			GetWorldTimerManager().ClearTimer(IdleActionTimerHandle);
		}
	}
}

void AMainCharacter::PlayIdleAction()
{
	IsIdleActionCouldPlay = true;
	IsIdleCouldPlay = false;
}

void AMainCharacter::PlayDefaultIdle()
{
	IsIdleCouldPlay = true;
}

FString AMainCharacter::GetPlayingAnimName()
{
	return GetNameSafe(AnimSequence);
}


void AMainCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	GetWorldTimerManager().ClearTimer(IdleActionTimerHandle);
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::Jump()
{
	bool IsFalling = GetCharacterMovement()->IsFalling();
	GetWorldTimerManager().ClearTimer(IdleActionTimerHandle);
	if(!IsFalling)
	{
		PlayAnim(JumpAnim);
	}
	Super::Jump();
}

void AMainCharacter::StopJumping()
{
	Super::StopJumping();
}

void AMainCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 3;
	IsRunning = true;
}

void AMainCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / 3;
	IsRunning = false;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SwitchAnim();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::StopJumping);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMainCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMainCharacter::StopRunning);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(ViewAction, ETriggerEvent::Started, this, &AMainCharacter::SwitchPerspective);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AMainCharacter::Zoom);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


