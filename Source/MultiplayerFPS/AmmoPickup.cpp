#include "AmmoPickup.h"
#include "Weapon.h"

void AAmmoPickup::Pickup(AFPSCharacter* CollidingCharacter) {
    TArray<AWeapon*> CharacterWeapons = CollidingCharacter->GetWeapons();

    for (int Index = 0 ; Index < CharacterWeapons.Num() ; Index++) {
        if (CharacterWeapons[Index]->GetWeaponType() == PickupWeaponType) {
            CollidingCharacter->AddAmmo(Index, PickupAmount);
        }
    }
}
