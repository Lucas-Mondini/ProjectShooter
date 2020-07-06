// Fill out your copyright notice in the Description page of Project Settings.

#include "botIAController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "botCharacter.h"
#include "Engine/Engine.h"
#include "Weapon.h"
#include "myPlayer.h"

AbotIAController::AbotIAController()
{
	//Construtor que cria o componente de sensores do bot inimigo
	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(FName("PawnSensingComponent"));
	//Aqui s�o os sensores
	pawnSensingComp->SensingInterval = 0.25f; //Fazer varredura sensorial a cada 0,25 segundos
	pawnSensingComp->bOnlySensePlayers = true; //Deve detectar apenas de Players(jogadores)
	pawnSensingComp->SetPeripheralVisionAngle(180.f); //�ngulo de vis�o deve ser 180 graus
	pawnSensingComp->SightRadius = 3000.f; //O raio de vis�o ser� de 3000 unidades

	behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent")); //Criamos um component BehaviorTree, processador de a��es
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackBoardComponent")); //Criamos um componente de BlackBoard(Manipular vari�veis de a��o)
}

void AbotIAController::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);
	//Se o evento onSeePawn acontecer, significa que o inimigo viu um Pawn(classe filha de Character)
	//Fazemos um "binding", amarramos uma fun��o do sensor da Unreal a nossa fun��o
	pawnSensingComp->OnSeePawn.AddDynamic(this, &AbotIAController::myOnSeePawn);

	if (behaviorTree)
	{
		//Iniciamos o BlackBoard e a BehaviorTree
		blackboardComp->InitializeBlackboard(*behaviorTree->BlackboardAsset);
		behaviorTreeComp->StartTree(*behaviorTree);

		//Setamos a vari�vel que existe no BlackBoard "Perambular" como true
		//Ou seja, se ele n�o viu o jogador ele fica "patrulhando"
		blackboardComp->SetValueAsBool("patrulhar", true);
	}

}

void AbotIAController::myOnSeePawn(APawn *sensedPawn)
{
	AbotCharacter *bot = Cast<AbotCharacter>(GetPawn());
	//  myOnseePawn recebe o objeto detectado no sensor do inimigo(botCharacter), mas sensedPawn retorna objeto da classe gen�rica
	// do player (pawn), fazemos um cast para a classe do nosso Player e colocar o resultado deste objeto especializado em Player
	AmyPlayer *player = Cast<AmyPlayer>(sensedPawn);
	//Somente se o inimigo estiver vida ele vai atirar e perseguir o player(jogador)


	// Se o inimigo estiver morto OU o Player estiver morto
	// Coloque enemy como NULO para parar o bot de perseguir e atirar
	if (bot->getHP() <= 0.0f || player->isDeadFlag())
	{
		blackboardComp->SetValueAsObject("enemy", nullptr);
		return;

	}
	if (bot->getHP() > 0.0f && !player->isDeadFlag())
	{
		if (blackboardComp && sensedPawn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Eu estou te vendo mano..."));

			//Setamos a vari�vel do BlackBoard Inimigo com o valor do objeto sensedPawn: Este objeto � o retornado quando o game
			//est� em execu��o e o sensor do inimigo detectar seu inimigo(no caso o player). 
			blackboardComp->SetValueAsObject("enemy", sensedPawn);

			//Quando isso � verdade, setamos "Patrulhar" como false, pois o inimigo viu o player e ele deve fazer outra coisa
			//(atirar por exemplo)
			blackboardComp->SetValueAsBool("patrulhar", false);

			//Precisamos obter uma refer�ncia ao objeto "Enemy"(botCharacter), que est� sendo possuido por este controlador(botIAController)
			//GetPawn() Devolve essa refer�ncia mas devolve via classegen�rica do tipo APawn. Como ACharacter � filha da classe APawn e BotCharacter � filho de ACharacter, podemos especificar este retorno
			//Usamos Cast para transformar o objeto gen�rico retornado do tipo APawn em AbotCharacter.		


			//Depois chamamos o m�todo atirar da arma deste bot inimigo
			bot->WeaponEnemy->shoot();
		}
	}
}
