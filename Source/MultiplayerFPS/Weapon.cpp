#include "Weapon.h"
#include "MultiplayerFPS.h"
#include "FPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon() {
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, FPSCharacter);
}

void AWeapon::SetOwner(AActor* NewOwner)  {
	Super::SetOwner(NewOwner);
	FPSCharacter = Cast<AFPSCharacter>(NewOwner);
}

void AWeapon::FireServer_Implementation() {
	Fire();
}

void AWeapon::Fire() {
	if (GetWorldTimerManager().IsTimerActive(FireTimer)) return;
	if (!FPSCharacter->HasWeaponEquipped()) return;
	int32 WeaponAmmo = FPSCharacter->GetEquippedWeaponAmmo();
	if (WeaponAmmo > 0) {
		FPSCharacter->ConsumeEquippedWeaponAmmo();
		if (FireAnimMontage) {
			FPSCharacter->PlayAnimMontageNetMulticast(FireAnimMontage);
		} else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, "Weapon FireAnimMontage is null");
		}
			
		GetWorldTimerManager().SetTimer(FireTimer, FireRate, false);

		FireHitscan();
		OnFire();
	} else {
		if (NoAmmoSound) {
			FPSCharacter->PlaySoundAtLocationClient(NoAmmoSound);
		}
	}
}

void AWeapon::FireHitscan() {
	FHitResult Hit; // FHitResult Hit(ForceInit);
	FVector Start = FPSCharacter->GetCameraLocation();  
	FVector End = FPSCharacter->GetCameraLocation() + FPSCharacter->GetCameraForwardVector() * HitscanDistance;
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(TArray<AActor*>({FPSCharacter}));
	UWorld* World = GetWorld();
	World->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	DrawDebugLine(World, Start, End, FColor::Emerald, false, 5.0f);
	AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(Hit.GetActor());
	if (HitCharacter) {
		HitCharacter->Damage(FPSCharacter, Damage);
	}
}

void AWeapon::SpawnProjectile() {
	
}