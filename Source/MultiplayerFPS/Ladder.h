#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Ladder.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ALadder : public AActor {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
public:	
	ALadder();
	
	UFUNCTION() 
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION() 
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
