#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenu.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleScoreboardVisibility();

	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreboardVisibility(bool bVisibility);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerKilled(const FString& KillerPlayerName, const FString& DeadPlayerName);
};
