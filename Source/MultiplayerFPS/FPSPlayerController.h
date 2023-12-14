#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "FPSPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TSubclassOf<class UPlayerMenu> PlayerMenuClass;
	UPROPERTY() class UPlayerMenu* PlayerMenu;
protected:
	virtual void BeginPlay() override;
public:
	void SetScoreboardVisibility(bool bVisibility);

	UFUNCTION(Client, Reliable) void ShowScoreboardClient();
	UFUNCTION(Client, Reliable) void NotifyKillClient(const FString& KillerPlayerName, const FString& DeadPlayerName);
};
