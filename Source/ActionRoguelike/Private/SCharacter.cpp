// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values

ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArm);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
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

void ASCharacter::FireProjectile(TSubclassOf<AActor> ProjectileClass)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd   = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParameters);

		if (ensure(AttackEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(AttackEffect, GetMesh(), "Muzzle_01");
		}
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	TimerDelegate_Attack.Unbind();
	TimerDelegate_Attack.BindUFunction(this, FName("FireProjectile"), PrimaryAttackClass);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDelegate_Attack, 0.2f, false);
}

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	TimerDelegate_Attack.Unbind();
	TimerDelegate_Attack.BindUFunction(this, FName("FireProjectile"), SecondaryAttackClass);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDelegate_Attack, 0.2f, false);
}

void ASCharacter::Teleport()
{
	PlayAnimMontage(AttackAnim);

	TimerDelegate_Attack.Unbind();
	TimerDelegate_Attack.BindUFunction(this, FName("FireProjectile"), TeleportProjectileClass);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDelegate_Attack, 0.2f, false);
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
		GetMesh()->SetScalarParameterValueOnMaterials("Time Of Hit Flash", GetWorld()->TimeSeconds);
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

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ASCharacter::Teleport);
}

// Old LOS firing code
// ensure(ProjectileClass);
// 	
// FVector  CameraLocation = CameraComp->GetComponentLocation();
// FRotator CameraRotation = CameraComp->GetComponentRotation();
// 	
// FVector Start = CameraLocation;
// FVector End   = Start + CameraRotation.Vector() * 3000.0f;
// 	
// FCollisionObjectQueryParams ObjectQueryParams;
// ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
// 	
// FHitResult Hit;
// bool bBlockingHit = GetWorld()->LineTraceSingleByProfile(Hit, Start, End, "Projectile");
// if (bBlockingHit)
// {
// 	End = Hit.Location;
// }
//
// const auto HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
// const auto HandRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
// FTransform SpawnTM = FTransform(HandRotation, HandLocation);
//
// FActorSpawnParameters SpawnParams = FActorSpawnParameters();
// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// SpawnParams.Instigator = this;
// 	
// GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
//
// // ---
// // const auto LineColor = bBlockingHit ? FColor::Green : FColor::Red;
// // DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);