#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"

#include "HealthPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AHealthPickup : public APickup {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) float PickupAmount;
public:
	
	virtual void Pickup(AFPSCharacter* CollidingCharacter) override;
};