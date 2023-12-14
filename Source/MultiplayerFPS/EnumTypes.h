#pragma once

// #include "CoreMinimal.h"

UENUM()
enum class EWeaponType : int32 {
    Pistol,
    MachineGun,
    Railgun,
    MAX
};

UENUM()
enum class EAmmoType : int32 {
    PistolBullets,
    MachineGunBullets,
    Slugs,
    MAX
};

UENUM()
enum class EWeaponFireMode: int32 {
    Single,
    Automatic,
    MAX
};