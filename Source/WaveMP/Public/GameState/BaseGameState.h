// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameState/GameStateEnum.h"
#include "BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class WAVEMP_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentGameState)
	EGameStatus CurrentGameState = EGameStatus::Waiting;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentGameTime)
	float GameTime = 0.f;

	UFUNCTION()
	void OnRep_CurrentGameState();

	UFUNCTION()
	void UpdateCurrentGameState(EGameStatus NewGameState);

	UFUNCTION()
	void OnRep_CurrentGameTime();
};
