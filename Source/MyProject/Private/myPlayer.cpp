// Fill out your copyright notice in the Description page of Project Settings.

#include "myPlayer.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Weapon.h"
#include "Engine/EngineTypes.h"
#include "Public/WorldCollision.h"


// Sets default values
AmyPlayer::AmyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//this->life = 100;
	this->ammo = 5000;

	//Estamos setando manualmente alguns par�metros que s�o dispon�veis via gr�fico, novamente apenas por praticidade
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 425.f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;
	AutoPossessPlayer = EAutoReceiveInput::Player0;//Par�metro respons�vel por setar a classe como jogador 1 (player1)

}

void AmyPlayer::addAmmo(int amount) {
	this->ammo += amount;
}

void AmyPlayer::moveX(float scale)
{
	//Fun��o para fazer o personagem se mover horizontalmente
	AddMovementInput(GetActorForwardVector() *  scale); //Escala por padr�o � 1.0 (setada via gr�fico em "inputs")
	//Observa��o: N�o foi adicionado uma fun��o para tratar velocidade extra "dash" para somar com a escala

}

void AmyPlayer::jump()
{
	//Se estiver pulando, isJumping verdadeiro
	isJumping = true;
}

void AmyPlayer::stopJump()
{
	//Se n�o estiver pulando, isJumping falso
	isJumping = false;
}

void AmyPlayer::moveY(float scale)
{
	//Fun��o para fazer o personagem se mover verticalmente
	AddMovementInput(GetActorRightVector() * scale);
}

void AmyPlayer::crouch()
{
	//Fun��o da Unreal para "agaichar" (efeito de c�mera apenas)
	Crouch();
}

void AmyPlayer::unCrouch()
{
	//Fun��o da Unreal para n�o "agaixar" (Efeito de c�mera apenas)
	UnCrouch();
}

void AmyPlayer::shoot()
{
	//Chamando uma fun��o atirar (shoot) da classe Weapon.cpp
	if (this->ammo > 0) {
		weaponPlayer->shoot();
		this->ammo--;
	}
}

void AmyPlayer::checkAmmo() {
	if (this->ammo > 500)
		this->ammo = 500;
	else if (this->ammo < 0)
		this->ammo = 0;
}


// Called when the game starts or when spawned
void AmyPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Estudar
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	weaponPlayer = GetWorld()->SpawnActor<AWeapon>(BP_Weapons, FTransform(), parameters);
	weaponPlayer->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("socketWeapon"));
}

// Called every frame
void AmyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	checkAmmo();
}

// Called to bind functionality to input
void AmyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Fun��o de manipula��o de inputs da Unreal
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Em aspas seria o nome que foi dado para os inputs graficametne, para cada a��o uma fun��o
	//BindAxis permite que quando o player mantenha pressionado a tecla, chame a fun��o enquanto pressionado
	PlayerInputComponent->BindAxis("btnMoveVertical", this, &AmyPlayer::moveX);
	PlayerInputComponent->BindAxis("btnMoveHorizontal", this, &AmyPlayer::moveY);
	PlayerInputComponent->BindAxis("mouseY", this, &AmyPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("mouseX", this, &AmyPlayer::AddControllerYawInput);
	

	//BindAction, permite que a fun��o seja chamada apenas quando � pressionada, quando o jogador continua segurando o bot�o, nada acontece
	//Um pouco parecido com "GetTicks" em SDL, por�m, aqui ao inves de ter um intervalo para acontecer novamente, s� acontece caso o player
	//solte e pressione novamente a tecla
	
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Pressed, this, &AmyPlayer::crouch);
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Released, this, &AmyPlayer::unCrouch);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Pressed, this, &AmyPlayer::jump);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Released, this, &AmyPlayer::stopJump);
	PlayerInputComponent->BindAction("leftMouse", EInputEvent::IE_Pressed, this, &AmyPlayer::shoot);//Podemos notar isso quando chamamos a fun��o "shoot", mesmo que voc� mantenha o bot�o esquerdo do mouse, as balas n�o saem, a n�o ser que solte
	//o b�t�o e pressione novamente
 
}

void AmyPlayer::setHP(float damage)
{
	if(hp > 0.0f)
	{
		hp -= damage;
	}
	else if (hp <= 0.0f)
	{
		isDead = true;
	}
	
}

float AmyPlayer::getHP()
{
	return hp;
}

bool AmyPlayer::isDeadFlag()
{
	return isDead;
}

