// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveAISpawner.generated.h"

UCLASS()
class WAVEMP_API AWaveAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimeBetweenWaves = 30.f;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
