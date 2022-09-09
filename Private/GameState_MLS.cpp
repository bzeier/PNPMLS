// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_MLS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"

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

}

void AGameState_MLS::Multicast_RespawnPlayer(ACharacter* Character)
{

}

void AGameState_MLS::KillPlayer_Implementation(ACharacter* Character, AController* Instigator)
{
	if (HasAuthority()) {
		IGameInterface::Execute_AddDeath(Character->GetPlayerState<UObject>());
		Multicast_KillPlayer(Character);
		if (Instigator) {
			BroadcastAddKillfeed(Character->GetPlayerState(), Instigator->GetPlayerState<APlayerState>());
			IGameInterface::Execute_AddKill(Instigator->GetPlayerState<UObject>());
		}
		else {
			BroadcastAddKillfeed(Character->GetPlayerState(), nullptr);
		}
		Character->GetController()->DisableInput(Character->GetController<APlayerController>());

		if (canRespawn) {
			RespawnQueue.Add(Character);
			Multicast_RespawnPlayer(RespawnQueue[0]);
			AGameModeBase* gamemode = GetDefaultGameMode<AGameModeBase>();
			IGameInterface::Execute_RespawnPlayer(gamemode<UObject>());
		}
	}
}

void AGameState_MLS::BroadcastAddKillfeed(APlayerState* Killed, APlayerState* Killer)
{

}


