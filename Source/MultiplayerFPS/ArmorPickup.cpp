#include "ArmorPickup.h"

void AArmorPickup::Pickup(AFPSCharacter* CollidingCharacter) {
    CollidingCharacter->AddArmor(PickupAmount);
}