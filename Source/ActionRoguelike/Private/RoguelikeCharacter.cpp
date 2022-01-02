// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"

#include "RoguelikeInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RoguelikeProjectileAbility.h"

ARoguelikeCharacter::ARoguelikeCharacter()
{
 	// Disable calling Tick() to improve performance
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<URoguelikeInteractionComponent>(TEXT("InteractionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void ARoguelikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	FName RightHandSocket = "Muzzle_01";
	UClass* PrimaryAttack = PrimaryAttackBP;
	TimerDelegate_PrimaryAttack.BindUFunction(this, FName("UseAbility_TimeElapsed"), RightHandSocket, PrimaryAttack);

	UClass* BlackHoleAbility = BlackHoleAbilityBP;
	TimerDelegate_BlackHoleAbility.BindUFunction(this, FName("UseAbility_TimeElapsed"), RightHandSocket, BlackHoleAbility);
}

void ARoguelikeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoguelikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARoguelikeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoguelikeCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARoguelikeCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARoguelikeCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAbility", IE_Pressed, this, &ARoguelikeCharacter::BlackHoleAbility);
}

void ARoguelikeCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ARoguelikeCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ARoguelikeCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();	
	}
}

void ARoguelikeCharacter::UseAbility_TimeElapsed(FName SpawnSocket, UClass* AbilityClass)
{
	// Raycast from camera to determine attack rotation adjustment for aim
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * 3000.0f;
	FHitResult Hit;
	FVector HitLoc;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
	{
		HitLoc = Hit.Location;
	} else
	{
		HitLoc = TraceEnd;
	}

	FVector RightHandLocation = GetMesh()->GetSocketLocation(SpawnSocket);

	FRotator RotationToCrosshair = (HitLoc - RightHandLocation).Rotation();
	
	FTransform SpawnTransform = FTransform(RotationToCrosshair, RightHandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(AbilityClass, SpawnTransform, SpawnParams);
}

void ARoguelikeCharacter::PrimaryAttack()
{
	PlayAnimMontage(PrimaryAttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate_PrimaryAttack, 0.2f, false);
}

void ARoguelikeCharacter::BlackHoleAbility()
{
	PlayAnimMontage(BlackHoleAbilityAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAbility, TimerDelegate_BlackHoleAbility, 0.2f, false);
}