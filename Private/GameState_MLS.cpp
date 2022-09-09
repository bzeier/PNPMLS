// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_MLS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AGameState_MLS::BeginPlay() {
	if (!IsRunningDedicatedServer()) {
		FString LevelName = GetWorld()->GetCurrentLevel()->GetName();
		if (LevelName == "MainMenu") {
			CreateWidget<UUserWidget>(this, WBP_MainMenu)->AddToViewport();
			APlayerController* pController = GetWorld()->GetFirstPlayerController();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "Lobby") {
			CreateWidget<UUserWidget>(this, WBP_Lobby)->AddToViewport();
			APlayerController* pController = GetWorld()->GetFirstPlayerController();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "GameMap") {
			CreateWidget<UUserWidget>(this, WBP_GameOverlay)->AddToViewport();
			CreateWidget<UUserWidget>(this, WBP_Loading)->AddToViewport();
			APlayerController* pController = GetWorld()->GetFirstPlayerController();
			pController->SetInputMode(FInputModeGameOnly());
			pController->SetShowMouseCursor(false);

		}
	}
}

void AGameState_MLS::StartGame_Implementation()
{
	if (HasAuthority()) {

	}
}

void AGameState_MLS::StartMatch_Implementation()
{
	if (HasAuthority()) {

	}
}

void AGameState_MLS::TogglePlayerInput(bool enable)
{

}

void AGameState_MLS::InitializePlayerArrayByKills()
{
}

void AGameState_MLS::UpdatePlayerlist_Implementation()
{
	Multicast_UpdatePlayerlist();
}

void AGameState_MLS::Multicast_UpdatePlayerlist()
{
	if (!IsRunningDedicatedServer()) {

	}
}

void AGameState_MLS::Multicast_KillPlayer(ACharacter* Character)
{
	Character->GetMesh()->SetSimulatePhysics(true);
	Character->GetMesh()->SetOwnerNoSee(true);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AGameState_MLS::Multicast_RespawnPlayer(ACharacter* Character)
{

}

void AGameState_MLS::KillPlayer_Implementation(ACharacter* Character, AController* Instigator)
{
	if (HasAuthority()) {
		IGameInterface::Execute_AddDeath(Character->GetPlayerState<UObject>());
		Multicast_KillPlayer(Character);
		Character->GetController()->DisableInput(Character->GetController<APlayerController>());

		if (Instigator) {
			BroadcastAddKillfeed(Character->GetPlayerState(), Instigator->GetPlayerState<APlayerState>());
			IGameInterface::Execute_AddKill(Instigator->GetPlayerState<UObject>());
			FindPlayerMatchPlaced(Instigator->GetPlayerState<APlayerState>());
			Multicast_SetMatchPlacedArray(PlayerArraySortedByKills);
			bool won;
			APlayerState* winner;
			MatchHasBeenWon(won, winner);
			if (won) {
				Winner = winner;
				IGameInterface::Execute_EndMatch(this);
			}

		}
		else {
			BroadcastAddKillfeed(Character->GetPlayerState(), nullptr);
		}
		

		if (canRespawn) {
			RespawnQueue.Add(Character);
			ACharacter* firstPlayer = RespawnQueue[0];
			Multicast_RespawnPlayer(firstPlayer);
			AGameMode* gamemode = Cast< AGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			IGameInterface::Execute_RespawnPlayer(gamemode, firstPlayer);
			IGameInterface::Execute_RespawnPlayer(firstPlayer, firstPlayer);
			RespawnQueue.RemoveSingle(firstPlayer);
		}
	}
}

void AGameState_MLS::BroadcastAddKillfeed(APlayerState* Killed, APlayerState* Killer)
{
	if (HasAuthority()) {
		for (APlayerState* p : PlayerArray) {
			IGameInterface::Execute_AddKillfeedMessage(p, Killed, Killer);
		}
	}
}

void AGameState_MLS::FindPlayerMatchPlaced(APlayerState* Player)
{
}

void AGameState_MLS::Multicast_SetMatchPlacedArray(TArray<APlayerState*> MatchPlacedArray)
{
}

void AGameState_MLS::MatchHasBeenWon(bool& HasBeenWon, APlayerState*& Winner)
{
}


