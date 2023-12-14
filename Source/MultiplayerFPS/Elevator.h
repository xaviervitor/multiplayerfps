#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AElevator : public AActor {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere)
	TArray<AElevator*> LinkedElevators;

	UPROPERTY(EditAnywhere);
	TArray<FTransform> ElevatorStops;

	int CurrentStopIndex = 0;

	float ActivationTime = 1.0f;

public:
	AElevator();

	UPROPERTY(EditAnywhere);
	float ElevatorSpeed = 1.0f;
protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void StartElevator();
	
	UFUNCTION()
	void StopElevator();

	void UpdateCurrentStop();
};
