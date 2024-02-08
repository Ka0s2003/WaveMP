// Copyright Michael Freeman

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Damage.h"
#include "WaveMPCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWaveMPAbilitySystemComponent;
class UBaseAbility;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AWaveMPCharacter : public ACharacter , public IDamage
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

public:
	AWaveMPCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetEnergy();

	UFUNCTION(BlueprintCallable)
	float GetMaxEnergy();

	UFUNCTION(BlueprintCallable)
	void ShootWeapon();

	UFUNCTION(Server, Reliable)
	void DamageActor(AActor* HitActor);

	UPROPERTY()
	class UDefaultAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

protected:

	UFUNCTION()
	void InitAttributesArray();
	
	UFUNCTION()
	void GiveAbilities();

	UFUNCTION(Client,Reliable)
	void SetInput();

	UFUNCTION(Server,Reliable)
	void HealthAtZero();

	UFUNCTION(BlueprintNativeEvent)
	void DeathEffects();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShotDamage = 25.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

