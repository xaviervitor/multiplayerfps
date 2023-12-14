#include "FPSGameState.h"

TArray<AFPSPlayerState*> AFPSGameState::GetPlayerStatesOrderedByKills() {
    TArray<AFPSPlayerState*> FPSPlayerStates;
    for (APlayerState* PlayerState : PlayerArray) {
        AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);
        FPSPlayerStates.Add(FPSPlayerState);
    }
    FPSPlayerStates.Sort([](const AFPSPlayerState& A, const AFPSPlayerState& B) {
        return A.GetKills() > B.GetKills();
    });
    return FPSPlayerStates;
}