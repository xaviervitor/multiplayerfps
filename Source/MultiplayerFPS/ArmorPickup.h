#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPSCharacter.h"

#include "ArmorPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AArmorPickup : public APickup {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) float PickupAmount;
public:
	virtual void Pickup(AFPSCharacter* CollidingCharacter) override;
};
