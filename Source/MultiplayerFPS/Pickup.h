#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSCharacter.h"

#include "Pickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API APickup : public AActor {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	class URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(EditAnywhere)
	class USoundBase* PickupSound;

	UPROPERTY(EditAnywhere)
	float ReactivationTime = 1.0f;
public:
	APickup();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION() 
	void OnBeginOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	virtual void Pickup(AFPSCharacter* CollidingCharacter) { return; }

	void DeactivatePickup();
	void ReactivatePickup();
};
