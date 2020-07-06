// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "botCharacter.generated.h"

UCLASS()
class MYPROJECT_API AbotCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AbotCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_EnemyWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float hp;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool isDead;
		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void setHP(float damage);

	UFUNCTION()
	float getHP();

	UFUNCTION()
	bool isDeadFlag();

	class AWeapon* WeaponEnemy;
	
	
};
