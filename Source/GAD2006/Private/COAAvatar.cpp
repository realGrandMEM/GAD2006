// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar() :
	RunSpeed(800),
	Stamina(100),
	MaxStamina(100),
	StaminaGainRate(8),
	StaminaDrainRate(5),
	bStaminaDrained(false),
	bRunning(false)

{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 300.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
}


// Called to bind functionality to input
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACOAAvatar::StopJumping);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACOAAvatar::RunReleased);


}

void ACOAAvatar::RunPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	bRunning = true;
	UpdateMovementParams();
	
}

void ACOAAvatar::RunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bRunning = false;
	UpdateMovementParams();
}


void ACOAAvatar::MoveForward(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, value);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("Move Forward: %f"), value));
}

void ACOAAvatar::MoveRight(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, value);

}
void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("Stamina: %f"), Stamina));
		if (Stamina <  0)
		{
			Stamina = 0;
			bStaminaDrained = true;
			UpdateMovementParams();
		}
		if (Stamina == MaxStamina)
		{
			bStaminaDrained = false;
			UpdateMovementParams();
		}
		if (bRunning)
		{
			if (bStaminaDrained)
			{
				bRunning = false;
				UpdateMovementParams();
			}
			else
			{
				bRunning = true;
				Stamina -= StaminaDrainRate * DeltaTime;
				UpdateMovementParams();
			}

		}
		if(!bRunning)
		{
			if (Stamina < 100)
			{
				Stamina += StaminaGainRate * DeltaTime;
			}
			else
			{
				Stamina = 100;
			}
		}
	}
	

}
void ACOAAvatar::UpdateMovementParams()
{
	if (!bStaminaDrained)
	{
		if (bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

}








