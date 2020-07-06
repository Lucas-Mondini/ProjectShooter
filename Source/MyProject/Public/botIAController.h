// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "botIAController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AbotIAController : public AAIController
{
	GENERATED_BODY()
	

protected:
	AbotIAController();

	//Dá o controle do Pawn(inimigo) ao Controller, ou seja, botCharacter será controlado pelo AbotIAController quando
	//for possuído(Possess) por este controlador
	virtual void Possess(APawn *InPawn) override;
	
	//SensingComponent tem configurações sensoriais(audição, visão, etc...) a Unreal é gente boa e deixa fácil pra nós manipular
	UPROPERTY(EditDefaultsOnly)
	class UPawnSensingComponent *pawnSensingComp;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree *behaviorTree;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTreeComponent *behaviorTreeComp;

	UPROPERTY(EditDefaultsOnly)
	class UBlackboardComponent *blackboardComp;

	UFUNCTION()
	void myOnSeePawn(class APawn *sensedPawn);

};
