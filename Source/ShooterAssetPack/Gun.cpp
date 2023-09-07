// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent= Root;

	Meshh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Meshh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Meshh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(ShootSound, Meshh, TEXT("MuzzleFlashSocket"));

	AController* OwnerController = GetOwnerController();
	if(!OwnerController) return;
	FHitResult HitResult;
	FVector ShotDirection;
	bool bSucces = SetLineTrace(HitResult, ShotDirection, OwnerController);
	if(bSucces){
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticle, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, HitResult.Location);
		AActor* HitActor = HitResult.GetActor();
		if(HitActor){
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		
	}
	

}

bool AGun::SetLineTrace(FHitResult &HitResult, FVector& ShotDirection, AController* OwnerController)
{
	
	FVector ControllerLocation;
	FRotator ControllerRotation;
	OwnerController->GetPlayerViewPoint(ControllerLocation, ControllerRotation);
	ShotDirection = -ControllerRotation.Vector();
	FVector End = ControllerLocation + ControllerRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerController->GetPawn());
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ControllerLocation,
		End,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params
	);
}

AController* AGun::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return nullptr;
	return OwnerPawn->GetController();

}
