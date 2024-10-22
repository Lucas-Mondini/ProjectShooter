// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "myPlayer.generated.h"

UCLASS()
class MYPROJECT_API AmyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AmyPlayer();
	void addAmmo(int amount);
	

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent *camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent *spring;
	*/

	

protected:
	// Called when the game starts or when spawned

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> BP_Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float hp;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool isDead;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
	bool isJumping;

	void jump();

	void stopJump();

	void moveY(float scale);

	void moveX(float scale);

	void crouch();

	void unCrouch();

	void shoot();


	virtual void BeginPlay() override;

public:	
	class AWeapon *weaponPlayer;


	void checkAmmo();




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


private:
	int ammo;
	
};
