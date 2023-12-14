#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPSCharacter.h"

#include "AmmoPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AAmmoPickup : public APickup {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) int32 PickupAmount;
	UPROPERTY(EditAnywhere) EWeaponType PickupWeaponType;
public:

	virtual void Pickup(AFPSCharacter* CollidingCharacter) override;
};
