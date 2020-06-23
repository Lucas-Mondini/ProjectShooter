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


// Sets default values
AmyPlayer::AmyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 425.f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;

	/*spring = CreateDefaultSubobject<USpringArmComponent>(FName("spring"));
	spring->TargetArmLength = 200.f;
	spring->bUsePawnControlRotation = true;
	spring->AddRelativeLocation(FVector(0.f, 40.f, 50.f));
	spring->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(FName("camera"));
	camera->SetupAttachment(spring);
	*/
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void AmyPlayer::moveX(float scale)
{
	AddMovementInput(GetActorForwardVector() *  scale);

}

void AmyPlayer::jump()
{
	isJumping = true;
}

void AmyPlayer::stopJump()
{
	isJumping = false;
}

void AmyPlayer::moveY(float scale)
{
	AddMovementInput(GetActorRightVector() * scale);
}

void AmyPlayer::crouch()
{
	Crouch();
}

void AmyPlayer::unCrouch()
{
	UnCrouch();
}


// Called when the game starts or when spawned
void AmyPlayer::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AWeapon *weaponPlayer = GetWorld()->SpawnActor<AWeapon>(BP_Weapons, FTransform(), parameters);

	weaponPlayer->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("socketWeapon"));
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
	
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Pressed, this, &AmyPlayer::crouch);
	PlayerInputComponent->BindAction("leftCTRL", EInputEvent::IE_Released, this, &AmyPlayer::unCrouch);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Pressed, this, &AmyPlayer::jump);
	PlayerInputComponent->BindAction("jump", EInputEvent::IE_Released, this, &AmyPlayer::stopJump);

	
	

}

