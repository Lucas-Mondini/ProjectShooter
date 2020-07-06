// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Public/CollisionQueryParams.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/SkeletalMeshActor.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"
#include "botCharacter.h"
#include "myPlayer.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	effectMuzzle = nullptr;
	impactGeral = nullptr;
	impactBlood = nullptr;
	decalImpactGeral = nullptr;

	//meshWeapon é uma classe do tipo SkeletalMesh != StaticMesh, nesse momento é criado um componente do tipo SkeletalMesh com nome MeshWeapon
	meshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MeshWeapon")); 

	//Serve para carregar uma malha(daria para fazer via gráfico, porém via código oferece praticidade, já que essa malha nunca mudará)
	//nesse momento meshObj tem consigo uma malha com texturas, shaders, etc..
	ConstructorHelpers::FObjectFinder<USkeletalMesh>meshObj(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	//Se a malha que foi setada na linha acima for um sucesso, entre.
	if (meshObj.Succeeded())
	{
		//Aqui de fato seta a malha da arma(textura,etc) para a meshweapon
		meshWeapon->SetSkeletalMesh(meshObj.Object);
	}
	
	// meshWeapon na hierarquia atua como pai
	RootComponent = meshWeapon;
	
	//Novamente foi criado uma outra mesh, ao invez de uma SkeletalMesh(mesh com esqueleto possível ter animações),
	//foi criado uma mesh do tipo flexa
	arrowWeapon = CreateDefaultSubobject<UArrowComponent>(FName("ArrowOfWeapon"));
	
	//Pega a flexa e anexa(acopla) em um outro componente existente da arma, no caso "MuzzleFlashSocket" seria a ponta da arma
	arrowWeapon->SetupAttachment(meshWeapon, FName("MuzzleFlashSocket"));
	arrowWeapon->SetRelativeLocation(FVector(1.5f, 0.f, -1.2f));//Posição em relação a posição da ponta da arma
	arrowWeapon->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f)); //Escala em relação a posição da ponta da arma
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

	//UArrowComponent *arrowWeapon = FindComponentByClass<UArrowComponent>();
	if (arrowWeapon)
	{
		//Pega a posição da "seta (arrowWeapon)" da arma, seta da arma
		FVector start = arrowWeapon->GetComponentLocation(); //Posição inicial da flecha
		FVector direction = arrowWeapon->GetComponentRotation().Vector(); //direção da flecha
		FVector end = start + (direction * 3000);

		/*O Raio de Raycast percorre até o fim, precisamos setar algo para o "hit" de impacto para 
		termos o resultado de que realmente o "raio" chegou no fim*/
		FHitResult infoImpact;
		FCollisionQueryParams parameters;
		
		//Não queremos que o raio coluda com a própria flexa (this)
		parameters.AddIgnoredActor(this);
		parameters.AddIgnoredActor(GetOwner()); // Ignora colisão com o dono da hierarquia desses componentes(A malha da arma)
		parameters.bTraceAsyncScene = true; // Analisar todos os vértices da colisão

		bool hit = GetWorld()->LineTraceSingleByChannel(infoImpact, start, end, ECollisionChannel::ECC_Visibility, parameters);

		if (hit)// se hit for diferente de 0 (falso)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Acertou em algo! Arriba!"));


			AActor *Ator = infoImpact.GetActor();
			//Se a classe do ator que o raio atingiu for do tipo ACharacter ou subclasse da mesma e impactblood !=0
			if (Ator->GetClass()->IsChildOf(ACharacter::StaticClass()) && impactBlood)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					impactBlood,
					infoImpact.Location,
					infoImpact.ImpactNormal.Rotation(),
					true);

				//Fazemos um cast para o objeto do tipo player
				// Se o ponteiro player não estiver vazio, significa que o cast deu certo e que o trace do tiro atingiu ou colidiu com um jogador
				AmyPlayer *player = Cast<AmyPlayer>(Ator);
				if (player != nullptr)
				{
					player->setHP(0.25f); // tira 0.25 do hp do player(jogador)
				}
				//Se não for um jogador o cast irá falhar e o ponteiro player será nulo e o código entrará aqui
				else
				{
					// Tentamos novamente o cast, só que para o tipo da classe pertencente ao objeto enemy
					// Logo se o resultado da colisão do line trace que está no Ator puder ser realizado este Cast, significa que
					// o tiro atingiu um inimigo e o ponteiro inimigo não será nulo
					AbotCharacter *enemy = Cast<AbotCharacter>(Ator);
					if (enemy != nullptr)
					{
						enemy->setHP(5.0f); // tira 5 de hp do inimigo
					}
				}
			}
			//Se não for inimigo, não queremos que seja sangue..
			else if (impactGeral)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					impactGeral,
					infoImpact.Location,
					infoImpact.ImpactNormal.Rotation(),
					true);

				FVector variableSize = FVector(FMath::RandRange(10.f, 50.f));
				
				/*UGameplayStatics::SpawnDecalAtLocation(decalImpactGeral,
					variableSize,
					infoImpact.GetComponent(),
					NAME_None);*/
			}
			
		}

		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f, (uint8)0, 1.0f);
			   



		if (effectMuzzle) // Fogo "explosão inicial" do tiro na arma
		{
			FVector location = arrowWeapon->GetComponentLocation();
			FRotator rotate = arrowWeapon->GetComponentRotation();
			FVector scale = FVector(0.9f);
			/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effectMuzzle, location, rotate, scale, true);*/
			UGameplayStatics::SpawnEmitterAttached(effectMuzzle,
													meshWeapon,
													NAME_None,
													meshWeapon->GetSocketLocation(FName("MuzzleFlashSocket")),
													arrowWeapon->GetComponentRotation(),
													EAttachLocation::KeepWorldPosition,
													true);
		

		}
		
	}
}

