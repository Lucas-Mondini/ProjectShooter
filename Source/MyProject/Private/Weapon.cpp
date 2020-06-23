// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "Public/CollisionQueryParams.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MeshWeapon"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>meshObj(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	if (meshObj.Succeeded())
	{
		meshWeapon->SetSkeletalMesh(meshObj.Object);
	}
	
	RootComponent = meshWeapon;
	
	arrowWeapon = CreateDefaultSubobject<UArrowComponent>(FName("ArrowOfWeapon"));
//anexar a seta em uma parte da arma
	arrowWeapon->SetupAttachment(meshWeapon, FName("MuzzleFlashSocket"));
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

void AWeapon::shoot()
{
	UArrowComponent *arrowWeapon = FindComponentByClass<UArrowComponent>();
	if (arrowWeapon)
	{
		//Pega a posição da "seta" da arma, seta da arma é um component e não um actor
		FVector start = arrowWeapon->GetComponentLocation();
		FVector direction = arrowWeapon->GetComponentRotation().Vector();
		FVector end = start + (direction * 1000);

		/*O Raio de Raycast percorre até o fim, precisamos setar algo para o "hit" de impacto para 
		termos o resultado de que realmente o "raio" chegou no fim*/
		FHitResult infoImpact;
		FCollisionQueryParams Parameters;
		//Não queremos que o raio coluda com a própria flexa (this)
		Parameters.AddIgnoredActor(this);
		Parameters.AddIgnoredActor(GetOwner()); // Ignora colisão com o dono da hierarquia desses componentes(A malha da arma)
		Parameters.bTraceAsyncScene = true; // Analisar todos os vértices da colisão

		bool hit = GetWorld()->LineTraceSingleByChannel(infoImpact, start, end, ECollisionChannel::ECC_Visibility, Parameters);

		if (hit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Acertou em algo!"));
		}

		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f, (uint8)0, 1.0f);

		
	}
}

