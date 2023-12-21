#include "Pickup.h"

#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

APickup::APickup() {
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("OverlapAll");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
	RootComponent = SphereComponent;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 90.0f, 0.0f);
	
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
}

// talvez apagar esse tick, não sei se precisa 
// dele pra funcionar o rotating movement component
void APickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
// talvez apagar esse tick, não sei se precisa 
// dele pra funcionar o rotating movement component

void APickup::OnBeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) {
	AFPSCharacter* CollidingCharacter = Cast<AFPSCharacter>(OtherActor);
	if (CollidingCharacter) {
		AController* PlayerController = CollidingCharacter->GetController();
		if (PlayerController && PlayerController->IsLocalController()) {
			DeactivatePickup();
			CollidingCharacter->PlaySoundAtLocationClient(PickupSound);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APickup::ReactivatePickup, ReactivationTime, false);
			Pickup(CollidingCharacter);
		}
	}
}

void APickup::DeactivatePickup() {
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void APickup::ReactivatePickup() {
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}