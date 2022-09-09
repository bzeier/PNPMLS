// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameInterface.h"
#include "GameState_MLS.generated.h"

/**
 * 
 */
UCLASS()
class PNPMLS_API AGameState_MLS : public AGameStateBase, public IGameInterface
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_MainMenu;
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_Lobby;
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_GameOverlay;
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_Loading;
		UPROPERTY(EditAnywhere)
			bool canRespawn = true;
		UPROPERTY(BlueprintReadOnly)
			TArray<ACharacter*> RespawnQueue;
		UPROPERTY(BlueprintReadOnly)
			TArray<APlayerState*> PlayerArraySortedByKills;
		bool canRespawn = true;
		UPROPERTY(BlueprintReadOnly)
			APlayerState* Winner;
		void BeginPlay();
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartGame();
		virtual void StartGame_Implementation();
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartMatch();
		virtual void StartMatch_Implementation();
		void TogglePlayerInput(bool enable);
		void InitializePlayerArrayByKills();
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdatePlayerlist();
		virtual void UpdatePlayerlist_Implementation();
		UFUNCTION(NetMulticast, Reliable)
		void Multicast_UpdatePlayerlist();
		UFUNCTION(NetMulticast, Reliable)
		void Multicast_KillPlayer(ACharacter* Character);
		UFUNCTION(NetMulticast, Reliable)
		void Multicast_RespawnPlayer(ACharacter* Character);
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void KillPlayer(ACharacter* Character, AController* Instigator);
		virtual void KillPlayer_Implementation(ACharacter* Character, AController* Instigator);
		void BroadcastAddKillfeed(APlayerState* Killed, APlayerState* Killer);
		void FindPlayerMatchPlaced(APlayerState* Player);
		UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetMatchPlacedArray(TArray<APlayerState*> MatchPlacedArray);
		void MatchHasBeenWon(bool& HasBeenWon, APlayerState*& Winner);
};
