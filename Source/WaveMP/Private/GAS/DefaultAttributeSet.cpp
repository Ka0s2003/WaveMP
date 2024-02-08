// Copyright Michael Freeman


#include "GAS/DefaultAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Character/WaveMPCharacter.h"

UDefaultAttributeSet::UDefaultAttributeSet()
{
	
}

void UDefaultAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefaultAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	
}

void UDefaultAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if(GetHealth() <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f,FColor::Blue, TEXT("Health is at ZERO"));
			if(Owner)
			{
				NotifyCharacterHealthAtZero();
			}
			else
			{
				SetNewOwner();
			}
		}
	}
	if(Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(),0.f,GetMaxEnergy()));
	}
}


void UDefaultAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Health, OldHealth);
}

void UDefaultAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxHealth, OldMaxHealth);
}

void UDefaultAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, Energy, OldEnergy);
}

void UDefaultAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefaultAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UDefaultAttributeSet::SetNewOwner()
{
	AActor* ComponentsOwner = GetOwningAbilitySystemComponent()->GetOwner();
	Owner = Cast<AWaveMPCharacter>(ComponentsOwner);
	if(Owner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f,FColor::Blue, TEXT("New Owner Set"));
		NotifyCharacterHealthAtZero();
	}
}

void UDefaultAttributeSet::NotifyCharacterHealthAtZero()
{
	if(Owner)
	{
		Owner->HealthAtZero_Implementation();
	}
}
