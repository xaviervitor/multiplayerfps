#include "Ladder.h"

#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ALadder::ALadder() {
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName("OverlapAll");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnEndOverlap);
}

void ALadder::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) {
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor);
	if (!FPSCharacter) return;
	FPSCharacter->bInLadder = true;
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(FPSCharacter->GetMovementComponent());
	if (!MovementComponent) return;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	
	// MovementComponent->
	// FPSCharacter->GetLastMovementInputVector()
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "in ladder");
	// MovementComponent->GravityScale = 0.0f;
}

void ALadder::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(OtherActor);
	if (!FPSCharacter) return;
	FPSCharacter->bInLadder = false;
	
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(FPSCharacter->GetMovementComponent());
	if (!MovementComponent) return;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "not in ladder");
	// MovementComponent->GravityScale = 1.0f;
}
