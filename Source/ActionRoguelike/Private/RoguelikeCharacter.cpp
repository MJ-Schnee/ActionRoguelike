// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCharacter.h"

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

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARoguelikeCharacter::OnHealthChanged);

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
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 5000.0f;

	FCollisionShape Shape;
	Shape.SetSphere(10.0f);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FHitResult Hit;
	if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
	{
		// Override TraceEnd for determining impact point
		TraceEnd = Hit.Location;
	}

	FVector HandLocation = GetMesh()->GetSocketLocation(SpawnSocket);

	FRotator RotationToCrosshair = (TraceEnd - HandLocation).Rotation();
	
	FTransform SpawnTransform = FTransform(RotationToCrosshair, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(AbilityClass, SpawnTransform, SpawnParams);
}

void ARoguelikeCharacter::PrimaryAttack()
{
	USceneComponent* CharacterMesh = GetMesh();
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, CharacterMesh, "Muzzle_01");
	
	PlayAnimMontage(PrimaryAttackAnim);

	FTimerHandle TimerHandle_PrimaryAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate_PrimaryAttack, 0.2f, false);
}

void ARoguelikeCharacter::BlackHoleAbility()
{
	USceneComponent* CharacterMesh = GetMesh();
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, CharacterMesh, "Muzzle_01");
	
	PlayAnimMontage(BlackHoleAbilityAnim);

	FTimerHandle TimerHandle_BlackHoleAbility;
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAbility, TimerDelegate_BlackHoleAbility, 0.2f, false);
}

void ARoguelikeCharacter::TeleportAbility()
{
	USceneComponent* CharacterMesh = GetMesh();
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, CharacterMesh, "Muzzle_02");
	
	PlayAnimMontage(TeleportAbilityAnim);

	FTimerHandle TimerHandle_TeleportAbility;
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAbility, TimerDelegate_TeleportAbility, 0.2f, false);
}

void ARoguelikeCharacter::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && NewHealth >= 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
		DisableInput(PlayerController);
	}
}
