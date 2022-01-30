// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TimeToHitParamName = "TimeOfHit";

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArm);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");
	
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::MoveForward(float Value)
{
	auto ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	auto ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	const auto RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, "SecondaryAttack");
}

void ASCharacter::Teleport()
{
	ActionComponent->StartActionByName(this, "Teleport");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		const auto playerController = Cast<APlayerController>(GetController());
		DisableInput(playerController);
	}

	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Gain rage from damage taken.
		const auto RageAmount = FMath::Abs(Delta) * RageConversionRate;
		const auto NewRage    = FMath::Clamp(Rage + RageAmount, 0.f, MaxRage);
		const auto RageDelta  = Rage - NewRage;

		Rage = NewRage;
		
		OnRageChanged.Broadcast(this, Rage, RageDelta);
	}
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ASCharacter::Teleport);
}

void ASCharacter::HealSelf(const float Amount /* 100 */)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

bool ASCharacter::SpendRage(const float Amount)
{
	if (!ensure(Amount > 0))
	{
		return false;
	}

	if (Rage < Amount)
	{
		// Not enough rage
		return false;
	}

	Rage -= Amount;
	OnRageChanged.Broadcast(this, Rage, Amount);
	return true;
}