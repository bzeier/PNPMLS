// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_MLS.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void AGameState_MLS::BeginPlay() {
	if (!IsRunningDedicatedServer()) {
		FString LevelName = GetWorld()->GetCurrentLevel()->GetName();
		APlayerController* pController = GetWorld()->GetFirstPlayerController();

		if (LevelName == "MainMenu") {
			WBP_MainMenu = CreateWidget<UUserWidget>(pController, WBP_MainMenu_Class);
			WBP_MainMenu->AddToViewport();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "Lobby") {
			WBP_Lobby = CreateWidget<UUserWidget>(pController, WBP_Lobby_Class);
			WBP_Lobby->AddToViewport();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "GameMap") {
			WBP_GameOverlay = CreateWidget<UUserWidget>(pController, WBP_GameOverlay_Class);
			WBP_GameOverlay->AddToViewport();
			WBP_Loading = CreateWidget<UUserWidget>(pController, WBP_Loading_Class);
			WBP_Loading->AddToViewport();
			pController->SetInputMode(FInputModeGameOnly());
			pController->SetShowMouseCursor(false);

		}
	}
}

void AGameState_MLS::StartGame_Implementation()
{
	if (HasAuthority()) {
		GetWorld()->GetTimerManager().SetTimer(DecreaseMatchBeginTimerHandle, this, &AGameState_MLS::DecreaseMatchBeginTimer, 1.0f, true);
	}
}

void AGameState_MLS::StartMatch_Implementation()
{
	if (HasAuthority()) {
		TogglePlayerInput(true);
		InitializePlayerArrayByKills();
		GetWorld()->GetTimerManager().SetTimer(DecreaseMatchTimerHandle, this, &AGameState_MLS::DecreaseMatchTimer, 1.0f, true);
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
	
	Character->GetMesh()->SetSimulatePhysics(false);
	Character->GetCapsuleComponent()->SetupAttachment(Character->GetMesh());
	FTransform MeshOffset = FTransform(FRotator(0,0, 270.0), FVector(0, -1.41291, -90.578857), FVector(1,1,1));
	Character->GetMesh()->SetRelativeTransform(MeshOffset);
	Character->GetMesh()->SetOwnerNoSee(true);
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AGameState_MLS::KillPlayer_Implementation(ACharacter* Character, AController* _instigator)
{
	if (HasAuthority()) {
		IGameInterface::Execute_AddDeath(Character->GetPlayerState<UObject>());
		Multicast_KillPlayer(Character);
		Character->GetController()->DisableInput(Character->GetController<APlayerController>());
		if (_instigator) {
			BroadcastAddKillfeed(Character->GetPlayerState(), _instigator->GetPlayerState<APlayerState>());
			IGameInterface::Execute_AddKill(_instigator->GetPlayerState<UObject>());
			FindPlayerMatchPlaced(_instigator->GetPlayerState<APlayerState>());
			Multicast_SetMatchPlacedArray(PlayerArraySortedByKills);
			bool won;
			APlayerState* winner = nullptr;
			MatchHasBeenWon(won, *winner);
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

void AGameState_MLS::Multicast_SetMatchPlacedArray(const TArray<APlayerState*> &MatchPlacedArray)
{
}

void AGameState_MLS::MatchHasBeenWon(bool& HasBeenWon, APlayerState& winner)
{
}

void AGameState_MLS::Multicast_DestroyWidgets()
{
	if (WBP_Lobby) {
		WBP_Lobby->RemoveFromParent();
	}
}

void AGameState_MLS::DecreaseMatchBeginTimer()
{
}

void AGameState_MLS::DecreaseMatchTimer()
{
}

void AGameState_MLS::Server_ReadyUp()
{
}

void AGameState_MLS::ReadyUp_Implementation()
{
	Server_ReadyUp();
}


