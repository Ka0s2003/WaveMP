// Copyright Michael Freeman


#include "Spawner/WaveAISpawner.h"

// Sets default values
AWaveAISpawner::AWaveAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AWaveAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

