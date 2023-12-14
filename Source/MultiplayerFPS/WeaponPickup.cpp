#include "WeaponPickup.h"

void AWeaponPickup::Pickup(AFPSCharacter* CollidingCharacter) {
	if (WeaponClass) {
		CollidingCharacter->AddWeaponServer(WeaponClass, WeaponAmmo);
	}
}