// Fill out your copyright notice in the Description page of Project Settings.

#include "myPlayer.h"
#include "Components/InputComponent.h"


// Sets default values
AmyPlayer::AmyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AmyPlayer::moveX(float scale)
{
	AddMovementInput(GetActorForwardVector() *  scale);

}

void AmyPlayer::moveY(float scale)
{
	AddMovementInput(GetActorRightVector() * scale);
}

// Called when the game starts or when spawned
void AmyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AmyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AmyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("btnMoveVertical", this, &AmyPlayer::moveX);
	PlayerInputComponent->BindAxis("btnMoveHorizontal", this, &AmyPlayer::moveY);
	PlayerInputComponent->BindAxis("mouseY", this, &AmyPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("mouseX", this, &AmyPlayer::AddControllerYawInput);

}

