#include "MultiplayerFPSGameModeBase.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayerState.h"
#include "FPSGameState.h"

AMultiplayerFPSGameModeBase::AMultiplayerFPSGameModeBase() {
    DefaultPawnClass = AFPSCharacter::StaticClass();
    PlayerControllerClass = AFPSPlayerController::StaticClass();
    PlayerStateClass = AFPSPlayerState::StaticClass();
    GameStateClass = AFPSGameState::StaticClass();
}

bool AMultiplayerFPSGameModeBase::ShouldSpawnAtStartSpot(AController* Player) {
    return false;
}

void AMultiplayerFPSGameModeBase::HandleMatchHasStarted() {
    Super::HandleMatchHasStarted();
    AFPSGameState* FPSGameState = Cast<AFPSGameState>(GameState);
    if (FPSGameState) {
        FPSGameState->SetKillLimit(KillLimit);
    }
}

void AMultiplayerFPSGameModeBase::HandleMatchHasEnded() {
    Super::HandleMatchHasEnded();
    TArray<AActor*> PlayerControllers;
    UGameplayStatics::GetAllActorsOfClass(this, AFPSPlayerController::StaticClass(), PlayerControllers);
    for (AActor* PlayerController : PlayerControllers) {
        AFPSPlayerController* FPSPlayerController = Cast<AFPSPlayerController>(PlayerController);
        if (!FPSPlayerController)
            continue;
        APawn* Pawn = FPSPlayerController->GetPawn();
        if (Pawn)
            Pawn->Destroy();
        FPSPlayerController->ShowScoreboardClient();
    }
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerFPSGameModeBase::RestartMap, 5.0f);
}

bool AMultiplayerFPSGameModeBase::ReadyToEndMatch_Implementation() {
    for (APlayerState* PlayerState : GameState->PlayerArray) {
        const AFPSPlayerState* FPSPlayerState = Cast<AFPSPlayerState>(PlayerState);
        if (FPSPlayerState && FPSPlayerState->GetKills() == KillLimit) {
            return true;
        }
    }
    return false;
}

void AMultiplayerFPSGameModeBase::RestartMap() const {
    GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}

void AMultiplayerFPSGameModeBase::OnDeath(AController* DeadPlayerController, AController* KillerPlayerController) {
        AFPSPlayerState* DeadPlayerState = Cast<AFPSPlayerState>(DeadPlayerController->PlayerState);
		AFPSPlayerState* KillerPlayerState = Cast<AFPSPlayerState>(KillerPlayerController->PlayerState);
		if (DeadPlayerState && KillerPlayerState) {
			DeadPlayerState->AddDeath();
			KillerPlayerState->AddKill();
		}

        AFPSPlayerController* KillerFPSController = Cast<AFPSPlayerController>(KillerPlayerController);
        KillerFPSController->NotifyKillClient(KillerPlayerController->PlayerState->GetPlayerName(), DeadPlayerController->PlayerState->GetPlayerName());
		
		// maybe this has to be a multicast respawn player?
		if (DeadPlayerController) {
			APawn* PlayerPawn = DeadPlayerController->GetPawn();
			if (PlayerPawn) PlayerPawn->Destroy();
			RestartPlayer(DeadPlayerController);
		}	
}

void AMultiplayerFPSGameModeBase::OnFall(AController* PlayerController) {
    AFPSPlayerState* PlayerState = Cast<AFPSPlayerState>(PlayerController->PlayerState);
	if (PlayerState) {
        PlayerState->AddDeath();
    }

	if (PlayerController) {
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn) PlayerPawn->Destroy();
		RestartPlayer(PlayerController);
	}
}
