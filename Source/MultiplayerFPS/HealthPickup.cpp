#include "HealthPickup.h"
#include "FPSCharacter.h"

void AHealthPickup::Pickup(AFPSCharacter* CollidingCharacter) {
    CollidingCharacter->AddHealth(PickupAmount);
}