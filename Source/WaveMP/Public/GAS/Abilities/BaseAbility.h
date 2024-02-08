// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseAbility.generated.h"

/**
 * 
 */
UCLASS()
class WAVEMP_API UBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
