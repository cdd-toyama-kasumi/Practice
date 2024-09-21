// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MainCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tools/log.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Construct();
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

void AMainCharacter::Construct()
{
	LogDefault("Construct");

	GetCapsuleComponent()->SetCapsuleHalfHeight(54.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	
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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Halo(TEXT("StaticMesh'/Game/Mine/Airi/StaticMesh/Airi_Halo.Airi_Halo'"));
	SkeletalMeshBody = LoadObject<USkeletalMesh>(nullptr,TEXT("SkeletalMesh'/Game/Mine/Airi/Airi_Character.Airi_Character'"));
	
	GetMesh()->SetRelativeLocation((FVector(0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight())));
	GetMesh()->SetSkeletalMesh(SkeletalMeshBody);
}

void AMainCharacter::FirstPerson() const
{
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->SetRelativeLocation(FVector(20.0f,0.0f,20.0f));
}

void AMainCharacter::ThirdPerson() const
{
	SpringArmComponent->TargetArmLength = 160.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f,-40.0f,20.0f));
}

void AMainCharacter::GenerateMainCamera()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, TEXT("SpringEndpoint"));
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
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	IsMove = true;
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
	IsMove = false;
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

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(ViewAction, ETriggerEvent::Started, this, &AMainCharacter::SwitchPerspective);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


