// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "weapon")
		class USkeletalMeshComponent *meshWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "weapon")
		class UArrowComponent *arrowWeapon;

	UPROPERTY(EditAnywhere, Category = effectsWeapon)
		class UParticleSystem *effectMuzzle;

	UPROPERTY(EditAnywhere, Category = effectsImpact)
		class UParticleSystem *impactBlood;

	UPROPERTY(EditAnywhere, Category = effectsImpact)
		class UParticleSystem *impactGeral;

	UPROPERTY(EditAnywhere, Category = effectsImpact)
		class UMaterialInterface *decalImpactGeral;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void shoot();

	
	
};
