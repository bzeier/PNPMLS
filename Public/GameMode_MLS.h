// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInterface.h"
#include "GameFramework/PlayerStart.h"
#include "GameMode_MLS.generated.h"

/**
 * 
 */
UCLASS()
class PNPMLS_API AGameMode_MLS : public AGameModeBase, public IGameInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* GamePawnClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString GameMapName = "";

	void BeginPlay();

	FVector GetSpawnStartLocation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndGame();
	virtual void EndGame_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TravelToGameMap();
	virtual void TravelToGameMap_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RespawnPlayer(ACharacter* Character);
	virtual void RespawnPlayer_Implementation(ACharacter* Character);
	void HandleStartingNewPlayer(APlayerController* NewPlayer);
	void Logout(AController* Exiting);
};
