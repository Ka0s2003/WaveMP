// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "GameState/GameStateEnum.h"
#include "GameFramework/GameModeBase.h"
#include "WaveMPGameMode.generated.h"

class APlayerStart;
class AWaveAISpawner;
class ABaseGameState;

UCLASS(minimalapi)
class AWaveMPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWaveMPGameMode();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartGameTimer();
	
protected:
	UFUNCTION()
	void WaitForGameStateToBeValid();

	UFUNCTION()
	void HandleWaitForGameState();
	
	UFUNCTION()
	void UpdateGameTimer();

	UFUNCTION()
	void CreateAISpawner();

	UFUNCTION(BlueprintCallable)
	void SetNewGameState(EGameStatus NewGameState);

	UFUNCTION()
	bool AnySurvivingPlayers();

	UFUNCTION()
	void SpawnDeadPlayers();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnNewPlayer(FVector Location, FRotator Rotation, ABaseController* Controller);

	UFUNCTION(BlueprintNativeEvent)
	void NotifyAllEndGame();

	UPROPERTY()
	bool FirstRound = true;

	UPROPERTY()
	TArray<AActor*> AllPlayerStarts;

	UPROPERTY()
	EGameStatus GameModeState = EGameStatus::Waiting;

	UPROPERTY()
	ABaseGameState* CurrentGameState = nullptr;

	UPROPERTY()
	FTimerHandle WaitForGameState;

	UPROPERTY()
	FTimerHandle GameTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI Spawner")
	TSubclassOf<AWaveAISpawner> SpawnerOfChoice;

	UPROPERTY()
	AWaveAISpawner* AISpawnerRef = nullptr;
};



