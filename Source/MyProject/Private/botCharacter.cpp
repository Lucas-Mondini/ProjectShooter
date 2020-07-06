// Fill out your copyright notice in the Description page of Project Settings.

#include "botCharacter.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AbotCharacter::AbotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AbotCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WeaponEnemy = GetWorld()->SpawnActor<AWeapon>(BP_EnemyWeapons, FTransform(), parameters);
	WeaponEnemy->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("socketWeapon"));
	
}

// Called every frame
void AbotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AbotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AbotCharacter::setHP(float damage)
{
	if (hp > 0.0f)
	{
		hp -= damage;
	}
	else if (hp <= 0.0f)
	{
		isDead = true;
		SetLifeSpan(10.f);
		WeaponEnemy->Destroy();
	}
}

float AbotCharacter::getHP()
{
	return hp;
}

bool AbotCharacter::isDeadFlag()
{
	return isDead;
}

