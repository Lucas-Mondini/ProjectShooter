// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh Weapon"));
	
	RootComponent = meshWeapon;
	arrowWeapon = CreateDefaultSubobject<UArrowComponent>(FName("Arrow of Weapon"));
	arrowWeapon-> AttachToComponent(meshWeapon, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("MuzzleFlashSocket"));
	arrowWeapon->SetRelativeLocation(FVector(1.5f, 0.f, -1.2f));
	arrowWeapon->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f));

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

