#include "FPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameMode.h"
#include "FPSPlayerState.h"
#include "MultiplayerFPS.h"
#include "FPSPlayerController.h"

AFPSCharacter::AFPSCharacter() {
 	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), "CameraSocket");
	CameraComponent->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	GetCharacterMovement()->JumpZVelocity = 460.0f;
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Ammo, COND_OwnerOnly);
	DOREPLIFETIME(AFPSCharacter, EquippedWeaponIndex);
	DOREPLIFETIME(AFPSCharacter, Weapons);
}

void AFPSCharacter::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		Health = MaxHealth;
		Armor = 0.0f;
		// PlaySoundAtLocationNetMulticast(SpawnSound);
	}

	GameMode = Cast<AMultiplayerFPSGameModeBase>(GetWorld()->GetAuthGameMode());	
}

void AFPSCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	// maybe a server rpc and then a netmulticast from the server?
	
	for (AWeapon* Weapon : Weapons) {
		Weapon->Destroy();
	}

	// it would be cool if the dead player dropped all the weapons instead
}

void AFPSCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}

void AFPSCharacter::FellOutOfWorld(const UDamageType& DmgType) {
	GameMode->OnFall(GetController());
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		EnhancedSubsystem->AddMappingContext(IMC_Player, 1);
		
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AFPSCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AFPSCharacter::Sprint);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AFPSCharacter::StopSprinting);
		
		EnhancedInputComponent->BindAction(IA_Scoreboard_UI, ETriggerEvent::Triggered, this, &AFPSCharacter::ShowScoreboard);
		EnhancedInputComponent->BindAction(IA_Scoreboard_UI, ETriggerEvent::Completed, this, &AFPSCharacter::HideScoreboard);
		
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AFPSCharacter::Fire);
		EnhancedInputComponent->BindAction(IA_PreviousWeapon, ETriggerEvent::Triggered, this, &AFPSCharacter::PreviousWeapon);
		EnhancedInputComponent->BindAction(IA_NextWeapon, ETriggerEvent::Triggered, this, &AFPSCharacter::NextWeapon);
		
		EnhancedInputComponent->BindAction(IA_Pistol, ETriggerEvent::Triggered, this, &AFPSCharacter::Pistol);
		EnhancedInputComponent->BindAction(IA_MachineGun, ETriggerEvent::Triggered, this, &AFPSCharacter::MachineGun);
		EnhancedInputComponent->BindAction(IA_Railgun, ETriggerEvent::Triggered, this, &AFPSCharacter::Railgun);
	}
}

void AFPSCharacter::Move(const FInputActionValue& Value) {
	const FVector2D InputVector = Value.Get<FVector2D>();
	if (Controller == nullptr || (InputVector.Length() == 0.0f)) return;

	// const FVector UpDirection = UKismetMathLibrary::GetUpVector(YawRotation);

	if (!bInLadder) {
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		const FVector ForwardDirection = YawRotation.Vector();
		AddMovementInput(ForwardDirection, InputVector.X);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(RightDirection, InputVector.Y);
	} else {
		FVector forward = GetCameraForwardVector();
		AddMovementInput(forward, InputVector.X);
		FVector right = GetCameraRightVector();
		AddMovementInput(right, InputVector.Y);
	}
}

void AFPSCharacter::Look(const FInputActionValue& Value) {
	const FVector2D InputVector = Value.Get<FVector2D>();
	if (InputVector.X != 0.0f) {
		AddControllerYawInput(InputVector.X); 
	}
	if (InputVector.Y != 0.0f) {
		AddControllerPitchInput(-InputVector.Y);
	}
}

void AFPSCharacter::Sprint(const FInputActionValue& Value) {
	// GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	// GetCharacterMovement()->JumpZVelocity = 460.0f;
}

void AFPSCharacter::StopSprinting(const FInputActionValue& Value) {
	// GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	// GetCharacterMovement()->JumpZVelocity = 420.0f;
}

void AFPSCharacter::SetScoreboardVisibility(bool bVisibility) {
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if(PlayerController) PlayerController->SetScoreboardVisibility(bVisibility);
}

void AFPSCharacter::ShowScoreboard(const FInputActionValue& Value) {
	SetScoreboardVisibility(true);
}

void AFPSCharacter::HideScoreboard(const FInputActionValue& Value) {
	SetScoreboardVisibility(false);
}

void AFPSCharacter::Fire(const FInputActionValue& Value) {
	AWeapon* EquippedWeapon = GetEquippedWeapon();
	if (EquippedWeapon) {
		EquippedWeapon->FireServer();
	}
}

void AFPSCharacter::Pistol(const FInputActionValue& Value) {
	EquipWeaponServer(0);
}

void AFPSCharacter::MachineGun(const FInputActionValue& Value) {
	EquipWeaponServer(1);
}

void AFPSCharacter::Railgun(const FInputActionValue& Value) {
	EquipWeaponServer(2);
}

void AFPSCharacter::PreviousWeapon(const FInputActionValue& Value) {
	PreviousWeaponServer();
}

void AFPSCharacter::NextWeapon(const FInputActionValue& Value) {
	NextWeaponServer();
}

void AFPSCharacter::PlayAnimMontageNetMulticast_Implementation(UAnimMontage* AnimMontage) {
	if (AnimMontage) {
		PlayAnimMontage(AnimMontage);
	}
}

void AFPSCharacter::PlaySoundAtLocationClient_Implementation(USoundBase* Sound) {
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
}

void AFPSCharacter::PlaySoundAtLocationNetMulticast_Implementation(USoundBase* Sound) {
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
	}
}

void AFPSCharacter::AddWeaponServer_Implementation(TSubclassOf<AWeapon> WeaponClass, int32 NewWeaponAmmo, bool bEquip) {
	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	if (NewWeapon) {
		Weapons.Add(NewWeapon);
		Ammo.Add(NewWeaponAmmo);
		NewWeapon->SetOwner(this);
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GripPointSocket");
		if (bEquip) {
			EquipWeaponServer(Weapons.Num() - 1);
		}
	}
}

void AFPSCharacter::EquipWeaponServer_Implementation(int32 Index) {
	if (Index == EquippedWeaponIndex) return;

	SwitchWeaponServer(Index);
}

void AFPSCharacter::NextWeaponServer_Implementation() {
	if (Weapons.Num() == 0) return;

	int32 NewWeaponIndex = EquippedWeaponIndex + 1;
	if (NewWeaponIndex > Weapons.Num() - 1) {
		NewWeaponIndex = 0;
	}
	if (EquippedWeaponIndex == NewWeaponIndex) return;
	SwitchWeaponServer(NewWeaponIndex);
}

void AFPSCharacter::PreviousWeaponServer_Implementation() {
	if (Weapons.Num() == 0) return;
	
	int32 NewWeaponIndex = EquippedWeaponIndex - 1;
	if (NewWeaponIndex < 0) {
		NewWeaponIndex = Weapons.Num() - 1;
	}
	if (EquippedWeaponIndex == NewWeaponIndex) return;
	SwitchWeaponServer(NewWeaponIndex);
}

void AFPSCharacter::SwitchWeaponServer_Implementation(int32 Index) {
	if (Weapons.IsValidIndex(Index)) {
		SwitchWeaponNetMulticast(Index);
		EquippedWeaponIndex = Index;
		PlaySoundAtLocationNetMulticast(WeaponSwitchSound);
	}
}

void AFPSCharacter::SwitchWeaponNetMulticast_Implementation(int32 Index) {
	if (Weapons.IsValidIndex(Index)) {
		AWeapon* EquippedWeapon = GetEquippedWeapon();
		if (EquippedWeapon) {
			EquippedWeapon->SetActorHiddenInGame(true);
		}
		GetWeapon(Index)->SetActorHiddenInGame(false);
	}
}

FVector AFPSCharacter::GetCameraLocation() {
	return CameraComponent->GetComponentLocation();
}

FVector AFPSCharacter::GetCameraForwardVector() {
	// return CameraComponent->GetForwardVector();
	return GetControlRotation().Vector();
}

FVector AFPSCharacter::GetCameraRightVector() {
	// return CameraComponent->GetForwardVector();
	return UKismetMathLibrary::GetRightVector(GetControlRotation());
}

void AFPSCharacter::AbsorbArmor(float Damage) {
	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(Armor, 0); 
	Armor -= Damage;
}

void AFPSCharacter::DropWeapon(int32 Index) {
	// drop on the ground;
	// Weapons[Index] = null;
	// Ammo[Index] = 0;
	// EquippedWeaponIndex = -1;
	// Weapons.RemoveAt(Index); ???
	// Ammo.RemoveAt(Index); ???
}

// After the character is damaged, it checks whether it’s dead.  If the character is not dead,
// then it plays the pain sound on the owning client.

void AFPSCharacter::Damage(AFPSCharacter* KillerCharacter, float Amount) {
	if (!IsAlive()) return;

	AbsorbArmor(Amount);
	RemoveHealth(Amount);

	if (IsAlive()) {
		PlayAnimMontageNetMulticast(DamageAnimMontage);
		PlaySoundAtLocationClient(DamageSound);
	} else {
		PlayAnimMontageNetMulticast(DeathAnimMontage);
		// TODO: wait for some seconds before respawning, so that the animation can play
		GameMode->OnDeath(GetController(), KillerCharacter->GetController());
	}
}

void AFPSCharacter::RemoveAmmo(int32 Index, int32 Amount) {
	int32 NewAmmo = Ammo[Index];
	NewAmmo -= Amount;
	if (NewAmmo < 0) NewAmmo = 0;
	Ammo[Index] = NewAmmo;
}