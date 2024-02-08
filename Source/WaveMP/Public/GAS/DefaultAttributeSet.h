// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DefaultAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class AWaveMPCharacter;

UCLASS()
class WAVEMP_API UDefaultAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UDefaultAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//setting health and bind to onrep function
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	//Set Max Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	//setting energy and bind to onrep function
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, Energy);
	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);
	
	//Set Max Energy
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UDefaultAttributeSet, MaxEnergy);
	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UFUNCTION()
	void SetNewOwner();

	UFUNCTION()
	void NotifyCharacterHealthAtZero();

private:
	UPROPERTY()
	AWaveMPCharacter* Owner = nullptr;
	

};
