#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Weapon.h"
#include "FPSCharacter.h"

#include "WeaponPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AWeaponPickup : public APickup {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(EditAnywhere) int32 WeaponAmmo;
public:
	virtual void Pickup(AFPSCharacter* CollidingCharacter) override;
};
