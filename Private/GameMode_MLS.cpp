// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_MLS.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"

void AGameMode_MLS::BeginPlay()
{
	if (GetWorld()->GetCurrentLevel()->GetName() == "GameMap") {
		IGameInterface::Execute_StartGame(GetGameState<UObject>());
	}
}

FVector AGameMode_MLS::GetSpawnStartLocation()
{
	FVector theResult = FVector(0.0f, 0.0f, 0.0f);
	FVector StartL;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*, FDefaultAllocator> IgnoreArray;
	ObjectTypesArray.Reserve(1);
	ObjectTypesArray.Emplace(ECollisionChannel::ECC_Pawn);
	FHitResult OutHit;
	bool hit;

	for (TObjectIterator<APlayerStart> Itr; Itr; ++Itr) {
		StartL = Itr->GetActorLocation();
		hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartL, StartL, 50.f, ObjectTypesArray, false, IgnoreArray, EDrawDebugTrace::ForDuration, OutHit, true);
		if (!hit) {
			return StartL;
		}
	}
	return theResult;
}


void AGameMode_MLS::TravelToGameMap_Implementation()
{
	GEngine->Exec(GetWorld(), TEXT("ServerTravel GameMap"));
}

void AGameMode_MLS::RespawnPlayer_Implementation(ACharacter* Character)
{
	Character->SetActorLocation(GetSpawnStartLocation());
	IGameInterface::Execute_RespawnPlayer(Character, Character);
	Character->GetController()->EnableInput(Character->GetController<APlayerController>());
}

void AGameMode_MLS::EndGame_Implementation()
{
	GEngine->Exec(GetWorld(), TEXT("ServerTravel Lobby"));
}

void AGameMode_MLS::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	FString LevelName = GetWorld()->GetMapName();

	if (LevelName == "MainMenu") {
		IGameInterface::Execute_SetActivePlayerController(GetGameInstance(), NewPlayer);
	}
	else if (LevelName == "Lobby") {
		IGameInterface::Execute_UpdatePlayerlist(GetGameState<UObject>());
		IGameInterface::Execute_CancelIntermissionTimer(GetGameState<UObject>());
	}else if (LevelName == "GameMap") {
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetSpawnStartLocation());
		IGameInterface::Execute_SpawnGamePawn(NewPlayer->GetPlayerState<UObject>(), GamePawnClass, SpawnTransform, NewPlayer);
		NewPlayer->GetPawn()->DisableInput(NewPlayer);
	}
}

void AGameMode_MLS::Logout(AController* Exiting) {
	FString reason;
	IGameInterface::Execute_GetDisconnectReason(Exiting->GetPlayerState<UObject>(), reason);
	IGameInterface::Execute_ShowPlayerAlert(Exiting->GetPlayerState<UObject>(), reason);
}

