// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/HtBaseCharacter.h"
#include "Gameplay/StatusEffect/HtStatusEffect.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values
AHtBaseCharacter::AHtBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHtBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHtBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHtBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}






#pragma region IHtStatusEffectContainer

TArray<AHtStatusEffect*> AHtBaseCharacter::GetStatusEffects(const TSubclassOf<AHtStatusEffect>& StatusEffectClass)
{
	TArray<AHtStatusEffect*> Result;

	for (AHtStatusEffect* Effect : StatusEffects)
	{
		if (Effect->IsA(StatusEffectClass))
		{
			Result.Add(Effect);
		}
	}

	return Result;
}

AHtStatusEffect* AHtBaseCharacter::CreateStatusEffect(const TSubclassOf<AHtStatusEffect>& StatusEffectClass)
{
	AHtStatusEffect* StatusEffect = GetWorld()->SpawnActor<AHtStatusEffect>(StatusEffectClass);
	check(StatusEffect != nullptr);

	StatusEffect->SetReplicates(true);
	StatusEffect->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	MARK_PROPERTY_DIRTY_FROM_NAME(AHtBaseCharacter, StatusEffects, this);
	StatusEffects.Add(StatusEffect);

	return StatusEffect;
}

#pragma endregion




void AHtBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AHtBaseCharacter, StatusEffects, Params);

}