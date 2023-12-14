#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "MultiplayerFPSGameModeBase.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameModeBase : public AGameMode {
	GENERATED_BODY()
	
	int32 KillLimit = 100;

	AMultiplayerFPSGameModeBase();
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	void RestartMap() const;

public:
	void OnDeath(AController* KillerController, AController* DeadPlayerController);
	void OnFall(AController* PlayerController);
};