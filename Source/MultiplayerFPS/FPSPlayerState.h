#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerState : public APlayerState {
	GENERATED_BODY()
	
	UPROPERTY(Replicated) int32 Kills = 0;
	UPROPERTY(Replicated) int32 Deaths = 0;
public:

	FORCEINLINE void AddKill() { Kills++; }
	FORCEINLINE void AddDeath() { Deaths++; }
	UFUNCTION(BlueprintCallable) int32 GetKills() const { return Kills; } 
	UFUNCTION(BlueprintCallable) int32 GetDeaths() const { return Deaths; }
};
