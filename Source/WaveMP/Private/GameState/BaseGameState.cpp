// Copyright Michael Freeman


#include "GameState/BaseGameState.h"

#include "Net/UnrealNetwork.h"

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseGameState, CurrentGameState);
	DOREPLIFETIME(ABaseGameState, GameTime);
}

void ABaseGameState::OnRep_CurrentGameState()
{
	//empty onrep
}

void ABaseGameState::UpdateCurrentGameState(EGameStatus NewGameState)
{
	CurrentGameState = NewGameState;
}

void ABaseGameState::OnRep_CurrentGameTime()
{
	//empty onrep
}
