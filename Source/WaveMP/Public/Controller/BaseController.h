// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseController.generated.h"

/**
 * 
 */
UCLASS()
class WAVEMP_API ABaseController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyPlayerDead();

	UFUNCTION(BlueprintCallable)
	void SetDead(bool Dead);

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool Dead = true;
	
protected:

	UFUNCTION(Client,Reliable)
	void SpawnHUD();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HudWidget;

	UPROPERTY()
	UUserWidget* CurrentSpawnedHUD = nullptr;
	
};
