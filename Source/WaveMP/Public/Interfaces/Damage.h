// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damage.generated.h"

class UGameplayEffect;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WAVEMP_API IDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DealDamageToPlayer(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DealDamageToAI(float DamageAmount);
};
