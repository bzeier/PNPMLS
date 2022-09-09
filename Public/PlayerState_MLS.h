// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameInterface.h"
#include "PlayerState_MLS.generated.h"

/**
 * 
 */
UCLASS()
class PNPMLS_API APlayerState_MLS : public APlayerState, public IGameInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ShowPlayerAlert(const FString& message);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetDisconnectReason(const FString& reason);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SpawnGamePawn(UClass* GamePawnClass, FTransform Transform, APlayerController* PlayerController);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ReadyUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddKill();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddDeath();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetPlayerScore(int score);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AddKillfeedMessage(APlayerState* Killed, APlayerState* Killer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CreateWinLooseWidget(bool win);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetMatchPlaced(int Placed);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetMatchConlusion();
};
