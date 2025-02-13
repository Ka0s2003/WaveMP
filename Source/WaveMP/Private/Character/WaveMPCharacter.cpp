// Copyright Michael Freeman


#include "Character/WaveMPCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Controller/BaseController.h"
#include "GAS/DefaultAttributeSet.h"
#include "GAS/WaveMPAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AWaveMPCharacter

AWaveMPCharacter::AWaveMPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UWaveMPAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UDefaultAttributeSet>("Attributes");
	
}

void AWaveMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWaveMPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitAttributesArray();
	GiveAbilities();
	SetInput();
	if (ABaseController* PlayerController = Cast<ABaseController>(Controller))
	{
		PlayerController->SetDead(false);
	}
}

float AWaveMPCharacter::GetHealth()
{
	if(Attributes)
	{
		return Attributes->GetHealth();	
	}
	return 0.f;
}

float AWaveMPCharacter::GetMaxHealth()
{
	if(Attributes)
	{
		return Attributes->GetMaxHealth();	
	}
	return 0.f;
}

float AWaveMPCharacter::GetEnergy()
{
	if(Attributes)
	{
		return Attributes->GetEnergy();	
	}
	return 0.f;
}

float AWaveMPCharacter::GetMaxEnergy()
{
	if(Attributes)
	{
		return Attributes->GetMaxEnergy();	
	}
	return 0.f;
}

void AWaveMPCharacter::ShootWeapon()
{
	AController* DefaultController = GetController();
	const APlayerController* PlayerController = Cast<APlayerController>(DefaultController);
	if(PlayerController != nullptr)
	{
		FHitResult HitResult;
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->PlayerCameraManager->GetCameraViewPoint(CameraLocation,CameraRotation);
		FVector TraceStart = CameraLocation;
		FVector TraceDirection = CameraRotation.Vector();
		FVector TraceEnd = TraceStart + TraceDirection * 50000.f;
		FCollisionQueryParams CollisionParams(FName(TEXT("LineTrace")), false, this);
		CollisionParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd, ECC_Camera, CollisionParams);
		FName AITag = "AI";
		if(HitResult.bBlockingHit && HitResult.GetActor()->ActorHasTag(AITag))
		{
			DamageActor(HitResult.GetActor());
		}
	}
}

void AWaveMPCharacter::DamageActor_Implementation(AActor* HitActor)
{
	if(HitActor->GetClass()->ImplementsInterface(UDamage::StaticClass()) && HasAuthority())
	{
		const IDamage* DamageInterface = Cast<IDamage>(HitActor);
		DamageInterface->Execute_DealDamageToAI(HitActor,ShotDamage);
	}
}


void AWaveMPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWaveMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWaveMPCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWaveMPCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWaveMPCharacter::InitAttributesArray()
{
	if(AbilitySystemComponent)
	{
		for(TSubclassOf<UGameplayEffect>& StartEffect : DefaultAttributes)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StartEffect, 1, EffectContext);
			if(SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void AWaveMPCharacter::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, -1, this));
		}
	}
}

void AWaveMPCharacter::SetInput_Implementation()
{
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AWaveMPCharacter::HealthAtZero_Implementation()
{
	if (ABaseController* PlayerController = Cast<ABaseController>(Controller))
	{
		PlayerController->NotifyPlayerDead();
		DeathEffects();
	}
}

void AWaveMPCharacter::DeathEffects_Implementation()
{
}

void AWaveMPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWaveMPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}