// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/GameSession.h"
#include "Blueprint/UserWidget.h"
#include "GameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PNPMLS_API IGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdatePlayerlist();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateSession();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void FindSessions();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndSession();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsPlayerReady();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShowPlayerAlert(const FString& message);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetDisconnectReason(const FString& reason);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetDisconnectReason(FString &Reason);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		AGameSession* GetSession();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CancelFindSessions();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool GetIsLookngForSessions();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SpawnGamePawn(UClass* GamePawnClass, FTransform Transform, APlayerController* PlayerController);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReadyUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetIntermissionTime(int &Time);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CancelIntermissionTimer();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CancelReadyUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TravelToGameMap();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartGame();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartMatch();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndMatch();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndGame();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetMatchTime(int& Time);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void HasSpawnedIn(bool& has);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void KillPlayer(ACharacter* Character, AController* _instigator);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RespawnPlayer(ACharacter* Character);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddKill();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetPlayerInfo(FName &Name, int &Kills, int &Deaths, int& Placed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddKillfeedMessage(APlayerState* Killed, APlayerState* Killer);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		//UUserWidget* GetGameOverlayWidget();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CreateWinLooseWidget(bool win);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetMatchBeginTime(int &Time);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetMatchConlusion();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void WipeMatchConclusion();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetMatchConclusion();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetPlayerScore();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetMatchPlaced(int Placed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetPlayerArrayByKill(TSubclassOf<APlayerState> PlayerArray);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void HUDShowHitmarker(bool Kill);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetQuickDebug(bool& quickDebug);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		APlayerController* GetActivePlayerController();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetActivePlayerController(APlayerController* PlayerController);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GetPlayerIsReady(bool& isReady);
};
