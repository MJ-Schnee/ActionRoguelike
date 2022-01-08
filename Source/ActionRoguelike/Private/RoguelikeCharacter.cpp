// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"

#include "RoguelikeAttributeComponent.h"
#include "RoguelikeInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RoguelikeProjectile.h"

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

	AttributeComp = CreateDefaultSubobject<URoguelikeAttributeComponent>(TEXT("AttributeComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

void ARoguelikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Primary attack timer and spawn setup
	FName RightHandSocket = "Muzzle_01";
	UClass* PrimaryAttack = PrimaryAttackBP;
	TimerDelegate_PrimaryAttack.BindUFunction(this, FName("UseAbility_TimeElapsed"), RightHandSocket, PrimaryAttack);

	// Black Hole ability timer and spawn setup
	UClass* BlackHoleAbility = BlackHoleAbilityBP;
	TimerDelegate_BlackHoleAbility.BindUFunction(this, FName("UseAbility_TimeElapsed"), RightHandSocket, BlackHoleAbility);
	
	// Teleport ability timer and spawn setup
	FName LeftHandSocket = "Muzzle_02";
	UClass* TeleportAbility = TeleportAbilityBP;
	TimerDelegate_TeleportAbility.BindUFunction(this, FName("UseAbility_TimeElapsed"), LeftHandSocket, TeleportAbility);
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
	PlayerInputComponent->BindAction("TeleportAbility", IE_Pressed, this, &ARoguelikeCharacter::TeleportAbility);
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

void ARoguelikeCharacter::TeleportAbility()
{
	PlayAnimMontage(TeleportAbilityAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAbility, TimerDelegate_TeleportAbility, 0.2f, false);
}
