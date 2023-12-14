#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnumTypes.h"
#include "Weapon.h"
#include "MultiplayerFPSGameModeBase.h"

#include "FPSCharacter.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter {
	GENERATED_BODY()
private:
	AFPSCharacter();
	UPROPERTY(EditAnywhere) class UCameraComponent* CameraComponent;
	UPROPERTY(EditAnywhere) class USoundBase* SpawnSound;
	UPROPERTY(EditAnywhere) class USoundBase* PlayerHitSound;
	UPROPERTY(EditAnywhere) class USoundBase* WeaponSwitchSound;
	UPROPERTY(EditAnywhere) class UAnimMontage* DamageAnimMontage;
	UPROPERTY(EditAnywhere) class USoundBase* DamageSound;
	UPROPERTY(EditAnywhere) class UAnimMontage* DeathAnimMontage;
	UPROPERTY(EditAnywhere) class USoundBase* LandSound;

	UPROPERTY(EditAnywhere) class UInputMappingContext* IMC_Player;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Move;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Look;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Sprint;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Scoreboard_UI;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Pistol;
	UPROPERTY(EditAnywhere) class UInputAction* IA_MachineGun;
	UPROPERTY(EditAnywhere) class UInputAction* IA_Railgun;
	UPROPERTY(EditAnywhere) class UInputAction* IA_PreviousWeapon;
	UPROPERTY(EditAnywhere) class UInputAction* IA_NextWeapon;

	UPROPERTY(EditAnywhere, Replicated) float Health;
	UPROPERTY(EditAnywhere, Replicated) float Armor;
	UPROPERTY(EditAnywhere) float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere) float MaxArmor = 100.0f;
	UPROPERTY(EditAnywhere) float ArmorAbsorption = 50.0f;

	UPROPERTY(EditAnywhere, Replicated) int32 EquippedWeaponIndex = -1;
	UPROPERTY(EditAnywhere, Replicated) TArray<AWeapon*> Weapons;
	UPROPERTY(EditAnywhere, Replicated) TArray<int32> Ammo;

	AMultiplayerFPSGameModeBase* GameMode;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetScoreboardVisibility(bool bVisibility);
	
	UFUNCTION() void Move(const FInputActionValue& Value);
	UFUNCTION() void Look(const FInputActionValue& Value);
	UFUNCTION() void Sprint(const FInputActionValue& Value);
	UFUNCTION() void StopSprinting(const FInputActionValue& Value);
	UFUNCTION() void ShowScoreboard(const FInputActionValue& Value);
	UFUNCTION() void HideScoreboard(const FInputActionValue& Value);
	UFUNCTION() void Fire(const FInputActionValue& Value);
	UFUNCTION() void Pistol(const FInputActionValue& Value);
	UFUNCTION() void MachineGun(const FInputActionValue& Value);
	UFUNCTION() void Railgun(const FInputActionValue& Value);
	UFUNCTION() void PreviousWeapon(const FInputActionValue& Value);
	UFUNCTION() void NextWeapon(const FInputActionValue& Value);
public:
	UFUNCTION(Client, Unreliable) void PlaySoundAtLocationClient(class USoundBase* Sound);
	UFUNCTION(NetMulticast, Unreliable) void PlaySoundAtLocationNetMulticast(class USoundBase* Sound);
	UFUNCTION(NetMulticast, Unreliable) void PlayAnimMontageNetMulticast(class UAnimMontage* AnimMontage);

	UFUNCTION(Server, Reliable) void AddWeaponServer(TSubclassOf<AWeapon> WeaponClass, int32 NewWeaponAmmo, bool bEquip = true);
	UFUNCTION(Server, Reliable) void EquipWeaponServer(int32 Index);
	UFUNCTION(Server, Reliable) void NextWeaponServer();
	UFUNCTION(Server, Reliable) void PreviousWeaponServer();
	UFUNCTION(Server, Reliable) void SwitchWeaponServer(int32 Index);
	UFUNCTION(NetMulticast, Reliable) void SwitchWeaponNetMulticast(int32 Index);

	FVector GetCameraLocation();
	FVector GetCameraForwardVector();
	FVector GetCameraRightVector();

	bool bInLadder = false;

	UFUNCTION(BlueprintPure) FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE void SetHealth(float NewHealth) { Health = NewHealth; }
	FORCEINLINE void AddHealth(float Amount) { Health += Amount; }
	FORCEINLINE void RemoveHealth(float Amount) { Health -= Amount; }
	void Damage(AFPSCharacter* KillerCharacter, float Amount);
	
	UFUNCTION(BlueprintPure) FORCEINLINE float GetArmor() { return Armor; }
	FORCEINLINE void SetArmor(float NewArmor) { Armor = NewArmor; }
	FORCEINLINE void AddArmor(float Amount) { Armor += Amount; }
	FORCEINLINE void AbsorbArmor(float Damage);

	UFUNCTION(BlueprintPure) FORCEINLINE AWeapon* GetWeapon(int32 Index) { return (Weapons.IsValidIndex(Index)) ? Weapons[Index] : nullptr; }
	void DropWeapon(int32 Index);

	UFUNCTION(BlueprintPure) FORCEINLINE int32 GetAmmo(int32 Index) { return Ammo[Index]; }
	FORCEINLINE void SetAmmo(int32 Index, int32 NewAmmo) { Ammo[Index] = NewAmmo; }
	FORCEINLINE void AddAmmo(int32 Index, int32 Amount) { Ammo[Index] += Amount; }
	void RemoveAmmo(int32 Index, int32 Amount);
	
	FORCEINLINE bool IsAlive() { return Health > 0; }
	
	FORCEINLINE bool HasWeaponEquipped() { return EquippedWeaponIndex != -1; }
	UFUNCTION(BlueprintPure) FORCEINLINE int32 GetEquippedWeaponIndex() { return EquippedWeaponIndex; }
	UFUNCTION(BlueprintPure) FORCEINLINE AWeapon* GetEquippedWeapon() { return (Weapons.IsValidIndex(EquippedWeaponIndex)) ? Weapons[EquippedWeaponIndex] : nullptr; }
	UFUNCTION(BlueprintPure) FORCEINLINE int32 GetEquippedWeaponAmmo() { return (Ammo.IsValidIndex(EquippedWeaponIndex)) ? Ammo[EquippedWeaponIndex] : 0; }
	FORCEINLINE void ConsumeEquippedWeaponAmmo() { if (Ammo.IsValidIndex(EquippedWeaponIndex)) Ammo[EquippedWeaponIndex]--; }

	UFUNCTION(BlueprintCallable) FORCEINLINE TArray<AWeapon*> GetWeapons() { return Weapons; }
};