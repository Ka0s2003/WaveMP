// Copyright Michael Freeman


#include "UI/BaseHUD.h"

#include "Character/WaveMPCharacter.h"

void UBaseHUD::NativeConstruct()
{
	if(AActor* Actor = this->GetOwningPlayerPawn())
	{
		OwningCharacter = Cast<AWaveMPCharacter>(Actor);
	}
	Super::NativeConstruct();
}
