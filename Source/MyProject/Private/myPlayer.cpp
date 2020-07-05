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

	this->life = 100;
	this->ammo = 50;

	//Estamos setando manualmente alguns parâmetros que são disponíveis via gráfico, novamente apenas por praticidade
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 425.f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;
	AutoPossessPlayer = EAutoReceiveInput::Player0;//Parâmetro responsável por setar a classe como jogador 1 (player1)

}

void AmyPlayer::addAmmo(int amount) {
	this->ammo += amount;
}

void AmyPlayer::moveX(float scale)
{
	//Função para fazer o personagem se mover horizontalmente
	AddMovementInput(GetActorForwardVector() *  scale); //Escala por padrão é 1.0 (setada via gráfico em "inputs")
	//Observação: Não foi adicionado uma função para tratar velocidade extra "dash" para somar com a escala

}

void AmyPlayer::jump()
{
	//Se estiver pulando, isJumping verdadeiro
	isJumping = true;
}

void AmyPlayer::stopJump()
{
	//Se não estiver pulando, isJumping falso
	isJumping = false;
}

void AmyPlayer::moveY(float scale)
{
	//Função para fazer o personagem se mover verticalmente
	AddMovementInput(GetActorRightVector() * scale);
}

void AmyPlayer::crouch()
{
	//Função da Unreal para "agaichar" (efeito de câmera apenas)
	Crouch();
}

void AmyPlayer::unCrouch()
{
	//Função da Unreal para não "agaixar" (Efeito de câmera apenas)
	UnCrouch();
}

void AmyPlayer::shoot()
{
	//Chamando uma função atirar (shoot) da classe Weapon.cpp
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
	//Função de manipulação de inputs da Unreal
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Em aspas seria o nome que foi dado para os inputs graficametne, para cada ação uma função
	//BindAxis permite que quando o player mantenha pressionado a tecla, chame a função enquanto pressionado
	PlayerInputComponent->BindAxis("btnMoveVertical", this, &AmyPlayer::moveX);
	PlayerInputComponent->BindAxis("btnMoveHorizontal", this, &AmyPlayer::moveY);
	PlayerInputComponent->BindAxis("mouseY", this, &AmyPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("mouseX", this, &AmyPlayer::AddControllerYawInput);
	

	//BindAction, permite que a função seja chamada apenas quando é pressionada, quando o jogador continua segurando o botão, nada acontece
	//Um pouco parecido com "GetTicks" em SDL, porém, aqui ao inves de ter um intervalo para acontecer novamente, só acontece caso o player
	//solte e pressione novamente a tecla
	
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Pressed, this, &AmyPlayer::crouch);
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Released, this, &AmyPlayer::unCrouch);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Pressed, this, &AmyPlayer::jump);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Released, this, &AmyPlayer::stopJump);
	PlayerInputComponent->BindAction("leftMouse", EInputEvent::IE_Pressed, this, &AmyPlayer::shoot);//Podemos notar isso quando chamamos a função "shoot", mesmo que você mantenha o botão esquerdo do mouse, as balas não saem, a não ser que solte
	//o bõtão e pressione novamente
 
}

