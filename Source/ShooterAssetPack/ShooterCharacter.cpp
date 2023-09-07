// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
    if(PlayerController){
        if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
            Subsystem->AddMappingContext(BaseMappingContext, 0);
        }
    }

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)){
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::MoveForward);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookUp);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Shoot);
	}

}

bool AShooterCharacter::IsDead() const
{
	if(Health>0){
		return false;
	}else{
		return true;
	}
}

float AShooterCharacter::GetHealthPercent() const
{
    return Health / MaxHealth;
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;

	if(IsDead())
	{
		AShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterGameModeBase>();
		if(GameMode){
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	}
	return DamageToApplied;
}

void AShooterCharacter::MoveForward(const FInputActionValue& Value)
{
	const FVector DirectionValue = Value.Get<FVector>();
	FVector DeltaLocation(0.f);
	AddMovementInput(GetActorForwardVector() * DirectionValue.X * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed) ;
	AddMovementInput(GetActorRightVector() *  DirectionValue.Y * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed);
	AddActorLocalOffset(DeltaLocation, true);
}

void AShooterCharacter::LookUp(const FInputActionValue& Value)
{

	const FVector DirectionValue = Value.Get<FVector>();
	FRotator DeltaRotation(0.f);
	AddControllerPitchInput(DirectionValue.Y * UGameplayStatics::GetWorldDeltaSeconds(this) * Sencibility);
	AddControllerYawInput(-DirectionValue.X * UGameplayStatics::GetWorldDeltaSeconds(this) * Sencibility);
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
