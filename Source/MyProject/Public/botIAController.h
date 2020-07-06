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

	//D� o controle do Pawn(inimigo) ao Controller, ou seja, botCharacter ser� controlado pelo AbotIAController quando
	//for possu�do(Possess) por este controlador
	virtual void Possess(APawn *InPawn) override;
	
	//SensingComponent tem configura��es sensoriais(audi��o, vis�o, etc...) a Unreal � gente boa e deixa f�cil pra n�s manipular
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
