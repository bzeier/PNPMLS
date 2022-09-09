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
			WBP_MainMenu = CreateWidget(GetWorld(), WBP_MainMenu_Class);
			WBP_MainMenu->AddToViewport();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "Lobby") {
			WBP_Lobby = CreateWidget(GetWorld(), WBP_Lobby_Class);
			WBP_Lobby->AddToViewport();
			pController->SetInputMode(FInputModeGameAndUI());
			pController->SetShowMouseCursor(true);

			IGameInterface::Execute_GetMatchConclusion(GetGameInstance<UObject>());

		} else if (LevelName == "GameMap") {
			WBP_GameOverlay = CreateWidget(GetWorld(), WBP_GameOverlay_Class);
			WBP_GameOverlay->AddToViewport();
			WBP_Loading = CreateWidget(GetWorld(), WBP_Loading_Class);
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

void AGameState_MLS::EndMatch_Implementation()
{
	if (HasAuthority()) {
		CreateMatchConclusions();
		TogglePlayerInput(false);

		if(Winner) IGameInterface::Execute_CreateWinLooseWidget(Winner, true);

		for (APlayerState* p : PlayerArray) {
			if (p != Winner) {
				IGameInterface::Execute_CreateWinLooseWidget(p, false);
			}
		}
		AGameMode* gamemode = Cast< AGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		IGameInterface::Execute_EndGame(gamemode);
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

void AGameState_MLS::Multicast_UpdatePlayerlist_Implementation()
{
	if (!IsRunningDedicatedServer()) {
		//WBP_Lobby->UpdatePlayerlist;
	}
}

void AGameState_MLS::Multicast_KillPlayer_Implementation(ACharacter* Character)
{
	Character->GetMesh()->SetSimulatePhysics(true);
	Character->GetMesh()->SetOwnerNoSee(true);
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AGameState_MLS::Multicast_RespawnPlayer_Implementation(ACharacter* Character)
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

void AGameState_MLS::Multicast_SetMatchPlacedArray_Implementation(const TArray<APlayerState*> &MatchPlacedArray)
{
	PlayerArraySortedByKills = MatchPlacedArray;
}

void AGameState_MLS::Multicast_MatchTime_Implementation(int time)
{
	MatchTime = time;
}

void AGameState_MLS::Multicast_MatchBeginTime_Implementation(int time)
{
	MatchBeginTime = time;
}

void AGameState_MLS::MatchHasBeenWon(bool& HasBeenWon, APlayerState& winner)
{
}

void AGameState_MLS::Multicast_DestroyWidgets_Implementation()
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
	MatchTime--;
	Multicast_MatchTime(MatchTime);
	if (MatchTime == 0) {
		DecreaseMatchTimerHandle.Invalidate();
		if (HasAuthority()) {
			IGameInterface::Execute_EndMatch(this);
		}
	}
}

void AGameState_MLS::Server_ReadyUp_Implementation()
{
	if (GetPlayerThatAreReady()) {
		StartIntermissionTimer();
	}
}

void AGameState_MLS::ReadyUp_Implementation()
{
	Server_ReadyUp();
}

void AGameState_MLS::CreateMatchConclusions()
{
}

void AGameState_MLS::CancelIntermissionTimer_Implementation()
{
	IntermissionTime = -1;
	if (IntermissionTimerHandle.IsValid()) {
		IntermissionTimerHandle.Invalidate();
	}

}

void AGameState_MLS::StartIntermissionTimer()
{
	IntermissionTime = IntermissionStartTime;
	GetWorld()->GetTimerManager().SetTimer(IntermissionTimerHandle, this, &AGameState_MLS::DecreaseIntermissionTimer, 1.0f, true);
}

void AGameState_MLS::DecreaseIntermissionTimer()
{
	IntermissionTime--;
	if (IntermissionTime == 0) {
		IntermissionTimerHandle.Invalidate();
		Multicast_DestroyWidgets();
		if (HasAuthority()) {
			AGameModeBase* gamemode = (UGameplayStatics::GetGameMode(GetWorld()));
			IGameInterface::Execute_TravelToGameMap(gamemode);
		}
	}
}

bool AGameState_MLS::GetPlayerThatAreReady()
{
	bool AllAreReady = true;
	bool isready;
	for (APlayerState* p : PlayerArray) {
		IGameInterface::Execute_GetPlayerIsReady(p, isready);
		if (!isready) {
			AllAreReady = false;
			return AllAreReady;
		}
	}

	return AllAreReady;
}


