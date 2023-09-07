// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    

    if(AIBehavior){
        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledShooterCharacter = Cast<AShooterCharacter>(GetPawn());

    if(ControlledShooterCharacter)
    {
        return ControlledShooterCharacter->IsDead();
    }

    return true;
}
