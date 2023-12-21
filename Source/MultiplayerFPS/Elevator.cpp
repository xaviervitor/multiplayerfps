#include "Elevator.h"

#include <assert.h>
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Algo/Reverse.h"
#include "ElevatorLatentAction.h"

AElevator::AElevator() {
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnBeginOverlap);
	RootComponent = BoxComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	ElevatorStops.Add(GetActorTransform());
}

void AElevator::BeginPlay() {
	Super::BeginPlay();
}

void AElevator::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) {
	if (!Cast<AFPSCharacter>(OtherActor)) return;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AElevator::StartElevator, ActivationTime, false);

	for (AElevator* LinkedElevator : LinkedElevators) {
		FTimerHandle LinkedTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LinkedTimerHandle, LinkedElevator, &AElevator::StartElevator, LinkedElevator->ActivationTime, false);
	}
}

void AElevator::StartElevator() {
	if (ElevatorStops.Num() < 2) return;
	BoxComponent->SetCollisionProfileName("NoCollision");
	
	//TODO: reutilize latent actions
	int32 UUID = FMath::RandRange(0, 1000000);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "StopElevator";
	LatentInfo.UUID = UUID;
	LatentInfo.Linkage = 0;

	// FIXME:
	//[CRASH]
	// Assertion failed: (Index >= 0) & (Index < ArrayNum) [File:C:\Program Files\Epic Games\UE_5.2\Engine\Source\Runtime\Core\Public\Containers\Array.h] [Line: 752] Array index out of bounds: 2 from an array of size 2
	// UnrealEditor_MultiplayerFPS!AElevator::StartElevator() [C:\Users\xavie\Documents\Unreal Projects\multiplayerfps\Source\MultiplayerFPS\Elevator.cpp:51]
	GetWorld()->GetLatentActionManager().AddNewAction(this, UUID, new ElevatorLatentAction(
		this, ElevatorStops[CurrentStopIndex], ElevatorStops[CurrentStopIndex + 1], GetWorld()->DeltaTimeSeconds, LatentInfo));
	CurrentStopIndex++;
}

void AElevator::StopElevator() {
	BoxComponent->SetCollisionProfileName("OverlapAll");
	
	int NextStop = CurrentStopIndex + 1;
	if (NextStop > ElevatorStops.Num() - 1) {
		Algo::Reverse(ElevatorStops);
		CurrentStopIndex = 0;
	}
}