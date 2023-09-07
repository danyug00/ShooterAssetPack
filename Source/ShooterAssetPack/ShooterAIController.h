// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERASSETPACK_API AShooterAIController : public AAIController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay();
	

public:
	virtual void Tick(float DeltaTime) override;
	
	bool IsDead() const;

private:

	APawn  * PlayerPawn;
	UPROPERTY(EditAnywhere, Category="AI")
	class UBehaviorTree* AIBehavior;

};
