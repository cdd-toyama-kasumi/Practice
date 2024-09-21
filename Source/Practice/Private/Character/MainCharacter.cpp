// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MainCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Construct();
}

void AMainCharacter::Construct()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(54.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	
	GenerateMainBody();
	GenerateMainCamera();
}

void AMainCharacter::GenerateMainBody()
{
	MeshComponentHalo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponentHalo"));
	SpringArmComponentHalo = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponentHalo"));
	
	SpringArmComponentHalo->TargetArmLength = 60.0f;
	SpringArmComponentHalo->SetupAttachment(GetCapsuleComponent());
	SpringArmComponentHalo->SetRelativeRotation(FRotator(-60.0f,0.0f,0.0f));
	MeshComponentHalo->SetupAttachment(SpringArmComponentHalo,TEXT("SpringEndpoint"));

	GetMesh()->SetRelativeLocation((FVector(0.0f,0.0f,-GetCapsuleComponent()->GetScaledCapsuleHalfHeight())));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> 
	//static UStaticMesh Halo = LoadObject<UStaticMesh>(nullptr,TEXT("StaticMesh'/Game/Character/Halo/SM_Halo.SM_Halo'"));
}

void AMainCharacter::GenerateMainCamera()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 160.0f;
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, TEXT("SpringEndpoint"));

	
	SkeletalMeshBody = LoadObject<USkeletalMesh>(nullptr,TEXT("SkeletalMesh'/Game/Mine/Body/SK_Body.SK_Body'"));
	GetMesh()->SetSkeletalMesh(SkeletalMeshBody);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


