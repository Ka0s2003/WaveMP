// Copyright Michael Freeman


#include "Gamemode/WaveMPGameMode.h"
#include "Character/WaveMPCharacter.h"
#include "Controller/BaseController.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Spawner/WaveAISpawner.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AWaveMPGameMode::AWaveMPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AWaveMPGameMode::BeginPlay()
{
	Super::BeginPlay();
	WaitForGameStateToBeValid();
	if(GetWorld())
	{
		UClass* PlayerStartClass = APlayerStart::StaticClass();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartClass, AllPlayerStarts);
	}
}

void AWaveMPGameMode::WaitForGameStateToBeValid()
{
	AGameStateBase* DefaultGameState = this->GetGameState<AGameStateBase>();
	CurrentGameState = Cast<ABaseGameState>(DefaultGameState);
	if(CurrentGameState == nullptr && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(WaitForGameState,this, &AWaveMPGameMode::HandleWaitForGameState,1.f,true);
	}
	else
	{
		CreateAISpawner();
	}
}

void AWaveMPGameMode::HandleWaitForGameState()
{
	AGameStateBase* DefaultGameState = this->GetGameState<AGameStateBase>();
	CurrentGameState = Cast<ABaseGameState>(DefaultGameState);
	if(CurrentGameState != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitForGameState);
		CreateAISpawner();
	}
}

void AWaveMPGameMode::StartGameTimer()
{
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &AWaveMPGameMode::UpdateGameTimer, 1.f, true);
	}
}

void AWaveMPGameMode::UpdateGameTimer()
{
	CurrentGameState->GameTime += 1.f;
	if(AnySurvivingPlayers())
	{
		SetNewGameState(EGameStatus::EndGame);
	}
}

void AWaveMPGameMode::CreateAISpawner()
{
	if(SpawnerOfChoice != nullptr && GetWorld())
	{
		const FVector Location = FVector(0.f,0.f,3000.f);
		const FRotator Rotation = FRotator(0.f,0.f,0.f);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AISpawnerRef = GetWorld()->SpawnActor<AWaveAISpawner>(SpawnerOfChoice, Location, Rotation, SpawnParameters);
		if(AISpawnerRef)
		{
			UE_LOG(LogTemp,Warning,TEXT("AI SPAWNER ALIVE!"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI SPAWNER FAILED TO CREATE"))
	}
}

void AWaveMPGameMode::SetNewGameState(EGameStatus NewGameState)
{
	GameModeState = NewGameState;
	if(CurrentGameState && GetWorld())
	{
		CurrentGameState->UpdateCurrentGameState(NewGameState);

		switch (NewGameState)
		{
		case EGameStatus::Waiting:
			GetWorld()->GetTimerManager().PauseTimer(GameTimer);
			if(AnySurvivingPlayers())
			{
				NotifyAllEndGame();
			}
			break;
		case EGameStatus::Playing:
			GetWorld()->GetTimerManager().UnPauseTimer(GameTimer);
			SpawnDeadPlayers();
			break;
		case EGameStatus::EndGame:
			GetWorld()->GetTimerManager().PauseTimer(GameTimer);
			NotifyAllEndGame();
			break;
		}
	}
}

bool AWaveMPGameMode::AnySurvivingPlayers()
{
	int NumPlayers = 0;
	if(CurrentGameState)
	{
		TArray<APlayerState*> Players = CurrentGameState->PlayerArray;
		for (APlayerState* PlayerState : Players)
		{
			ABaseController* tController = Cast<ABaseController>(PlayerState->GetPlayerController());
			if(tController && AllPlayerStarts.Num() > 0)
			{
				if (const bool bIsDead = tController->Dead)
				{
					NumPlayers += 1;
				}
			}
		}
		return NumPlayers >= Players.Num();
	}
	return false;
}

void AWaveMPGameMode::SpawnDeadPlayers()
{
	if(CurrentGameState)
	{
		TArray<APlayerState*> Players = CurrentGameState->PlayerArray;
		for (APlayerState* PlayerState : Players)
		{
			ABaseController* tController = Cast<ABaseController>(PlayerState->GetPlayerController());
			if(tController && AllPlayerStarts.Num() > 0)
			{
				if (const bool bIsDead = tController->Dead)
				{
					const int RandomIndex = FMath::RandRange(0, AllPlayerStarts.Num() -1);
					const AActor* RandomSpawnPoint = AllPlayerStarts[RandomIndex];
					const FVector SpawnLocation = RandomSpawnPoint->GetActorLocation();
					const FRotator SpawnRotation(0.f,0.f,0.f);
					SpawnNewPlayer(SpawnLocation, SpawnRotation, tController);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NO SPAWN POINTS"))
			}
		}
	}
}

void AWaveMPGameMode::NotifyAllEndGame_Implementation()
{
	//used in BP to notify all the game ended
}

void AWaveMPGameMode::SpawnNewPlayer_Implementation(FVector Location, FRotator Rotation, ABaseController* Controller)
{
	//used in BP to spawn a new player
}
