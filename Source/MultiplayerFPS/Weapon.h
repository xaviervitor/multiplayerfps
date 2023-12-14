#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypes.h"

#include "Weapon.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AWeapon : public AActor {
	GENERATED_BODY()
private:
	UPROPERTY(Replicated) class AFPSCharacter* FPSCharacter;
	UPROPERTY(EditAnywhere) class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere) class USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditAnywhere) FString WeaponName;
	UPROPERTY(EditAnywhere) EWeaponType WeaponType;
	UPROPERTY(EditAnywhere) EAmmoType AmmoType;
	UPROPERTY(EditAnywhere) EWeaponFireMode FireMode;
	UPROPERTY(EditAnywhere) float HitscanDistance;
	UPROPERTY(EditAnywhere) float Damage;
	UPROPERTY(EditAnywhere) float FireRate;
	UPROPERTY(EditAnywhere) class UAnimMontage* FireAnimMontage;
	UPROPERTY(EditAnywhere) class USoundBase* NoAmmoSound;
	UPROPERTY(EditAnywhere) FTimerHandle FireTimer;
public:
	AWeapon();

	virtual void SetOwner(AActor* NewOwner) override;
	UFUNCTION(Server, Reliable) void FireServer();
	
protected:
	void Fire();
	void FireHitscan();
	void SpawnProjectile();
public:
	UFUNCTION(BlueprintPure) FORCEINLINE FString GetName() { return WeaponName; }

	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; }
};