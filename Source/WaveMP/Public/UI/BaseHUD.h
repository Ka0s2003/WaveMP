// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseHUD.generated.h"

class AWaveMPCharacter;
/**
 * 
 */
UCLASS()
class WAVEMP_API UBaseHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	AWaveMPCharacter* OwningCharacter = nullptr;
	
};
