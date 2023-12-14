#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSPlayerState.h"
#include "FPSGameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSGameState : public AGameState {
	GENERATED_BODY()
public:
	int32 KillLimit = 0;

	TArray<AFPSPlayerState*> GetPlayerStatesOrderedByKills();

	FORCEINLINE void SetKillLimit(int32 NewKillLimit) { KillLimit = NewKillLimit; }
};
