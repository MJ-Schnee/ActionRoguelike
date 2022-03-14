// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"

#include "RoguelikeActionComponent.h"
#include "RoguelikeAttributeComponent.h"
#include "RoguelikeInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RoguelikeProjectile.h"
#include "Kismet/GameplayStatics.h"

ARoguelikeCharacter::ARoguelikeCharacter()
{
	// Disable calling Tick() to improve performance
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<URoguelikeInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<URoguelikeAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<URoguelikeActionComponent>("ActionComp");
}

void ARoguelikeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARoguelikeCharacter::OnHealthChanged);
}

FVector ARoguelikeCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARoguelikeCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARoguelikeCharacter::SprintStop);

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

void ARoguelikeCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ARoguelikeCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ARoguelikeCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ARoguelikeCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ARoguelikeCharacter::BlackHoleAbility()
{
	ActionComp->StartActionByName(this, "BlackHoleAbility");
}

void ARoguelikeCharacter::TeleportAbility()
{
	ActionComp->StartActionByName(this, "TeleportAbility");
}

void ARoguelikeCharacter::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
                                          float NewHealth, float Delta)
{
	if (Delta < 0.0f && NewHealth >= 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);

		SetLifeSpan(5.0f);
	}
}
