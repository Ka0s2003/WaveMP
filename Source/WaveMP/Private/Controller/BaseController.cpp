// Copyright Michael Freeman


#include "Controller/BaseController.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "UI/BaseHUD.h"

void ABaseController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SpawnHUD();
}

void ABaseController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseController, Dead)
}

void ABaseController::SetDead(bool bDead)
{
	Dead = bDead;
}

void ABaseController::NotifyPlayerDead_Implementation()
{
	Dead = true;
	//Continue this in BP
}

void ABaseController::SpawnHUD_Implementation()
{
	if(CurrentSpawnedHUD != nullptr)
	{
		CurrentSpawnedHUD->RemoveFromParent();
	}
	if(HudWidget)
	{
		CurrentSpawnedHUD = CreateWidget<UUserWidget>(this, HudWidget);
		if(CurrentSpawnedHUD != nullptr)
		{
			CurrentSpawnedHUD->AddToViewport();
		}
	}
}
