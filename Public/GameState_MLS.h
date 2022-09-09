// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameInterface.h"
#include "Blueprint/UserWidget.h"
#include "GameState_MLS.generated.h"

/**
 * 
 */
UCLASS()
class PNPMLS_API AGameState_MLS : public AGameStateBase, public IGameInterface
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_MainMenu_Class;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_Lobby_Class;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_GameOverlay_Class;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WBP_Loading_Class;

		UPROPERTY(BlueprintReadOnly)
			UUserWidget* WBP_MainMenu;

		UPROPERTY(BlueprintReadOnly)
			UUserWidget* WBP_Lobby;

		UPROPERTY(BlueprintReadOnly)
			UUserWidget* WBP_GameOverlay;

		UPROPERTY(BlueprintReadOnly)
			UUserWidget* WBP_Loading;

		UPROPERTY(EditAnywhere)
			bool canRespawn = true;

		UPROPERTY(BlueprintReadOnly)
			TArray<ACharacter*> RespawnQueue;
		UPROPERTY(BlueprintReadOnly)
			TArray<APlayerState*> PlayerArraySortedByKills;

		UPROPERTY(BlueprintReadOnly)
			FTimerHandle DecreaseMatchBeginTimerHandle;
		UPROPERTY(BlueprintReadOnly)
			FTimerHandle DecreaseMatchTimerHandle;

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
		void KillPlayer(ACharacter* Character, AController* _instigator);
		virtual void KillPlayer_Implementation(ACharacter* Character, AController* _instigator);
		void BroadcastAddKillfeed(APlayerState* Killed, APlayerState* Killer);
		void FindPlayerMatchPlaced(APlayerState* Player);

		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetMatchPlacedArray(const TArray<APlayerState*> &MatchPlacedArray);
			virtual void Multicast_SetMatchPlacedArray_Implementation(const TArray<APlayerState*>& MatchPlacedArray);

		void MatchHasBeenWon(bool& HasBeenWon, APlayerState& winner);
		
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_DestroyWidgets();
			virtual void Multicast_DestroyWidgets_Implementation();

		void DecreaseMatchBeginTimer();
		void DecreaseMatchTimer();

		UFUNCTION(Server, Reliable)
			void Server_ReadyUp();
			virtual void Server_ReadyUp_Implementation();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReadyUp();
		virtual void ReadyUp_Implementation();
};
