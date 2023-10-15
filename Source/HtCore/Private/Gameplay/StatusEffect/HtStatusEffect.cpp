// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StatusEffect/HtStatusEffect.h"
#include "Gameplay/StatusEffect/HtStatusEffectContainer.h"

#include "Blueprint/UserWidget.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
AHtStatusEffect::AHtStatusEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f / AHtStatusEffect::EffectTickFreqHz;
}

// Called when the game starts or when spawned
void AHtStatusEffect::BeginPlay()
{
	Super::BeginPlay();

#ifndef WITH_SERVER_CODE
	if (bApplied) // We were replicated to an actor joining the server and this effect was already applied, trigger the animation
	{
		ApplyEffectCosmetics();
	}
#endif
}

// Called every frame
void AHtStatusEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bApplied)
	{
		EffectDuration--;

#ifdef WITH_SERVER_CODE
		if (EffectDuration == 0)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(AHtStatusEffect, EffectDuration, this); // Replicate that we're at 0 effect duration
			Remove();
		}
#endif
	}
}

inline IHtStatusEffectContainer* AHtStatusEffect::GetContainer() const { return Cast<IHtStatusEffectContainer>(GetEffectTarget()); }
inline AActor* AHtStatusEffect::GetEffectTarget() const { AActor* Parent = GetAttachParentActor(); check(Parent != nullptr); return Parent; }

void AHtStatusEffect::ApplyInternal_Implementation(int32 Duration, int32 Magnitude)
{
	if (bApplied)
		return;

	RepSetEffectDuration((uint16)Duration);
	RepSetEffectMagnitude((uint16)Magnitude);

	if (GEngine->GetNetMode(GetWorld()) < ENetMode::NM_Client)
	{
		bApplied = true;
		MARK_PROPERTY_DIRTY_FROM_NAME(AHtStatusEffect, bApplied, this);
		ApplyEffect();
	}

	BpApplyEffectCosmetics();
}

void AHtStatusEffect::RemoveInternal_Implementation()
{
	if (!bApplied)
		return;

	if (GEngine->GetNetMode(GetWorld()) < ENetMode::NM_Client)
	{
		bApplied = false;
		MARK_PROPERTY_DIRTY_FROM_NAME(AHtStatusEffect, bApplied, this);

		UnapplyEffect();
		BpRemoveEffectCosmetics();
		BpRemoveEffect();

		return;
	}

	BpRemoveEffectCosmetics();
}


void AHtStatusEffect::Remove() { RemoveInternal(); }
bool AHtStatusEffect::Apply(int32 Duration, int32 Magnitude)
{
	// If we're already applied OR we can't be applied b/c of the presence of another status effect, return false
	if (bApplied || !GetContainer()->CanApplyStatusEffect(StaticClass()))
		return false;

	ApplyInternal(Duration, Magnitude); // Actually apply the effect

	return true;
}

void AHtStatusEffect::RepSetEffectDuration(uint16 NewValue)
{
	if (EffectDuration != NewValue)
	{
		EffectDuration = NewValue;
		MARK_PROPERTY_DIRTY_FROM_NAME(AHtStatusEffect, EffectDuration, this);
	}
}

void AHtStatusEffect::RepSetEffectMagnitude(uint16 NewValue)
{
	if (EffectMagnitude != NewValue)
	{
		EffectMagnitude = NewValue;
		MARK_PROPERTY_DIRTY_FROM_NAME(AHtStatusEffect, EffectMagnitude, this);
	}
}

void AHtStatusEffect::Update(int32 Duration, int32 Magnitude)
{
	if (!bApplied)
	{
		RepSetEffectDuration((uint16)Duration);
		RepSetEffectMagnitude((uint16)Magnitude);
		return;
	}


	if (EffectMagnitude != Magnitude ||
		EffectDuration != Duration)
	{
		UnapplyEffect();
		RepSetEffectDuration((uint16)Duration);
		RepSetEffectMagnitude((uint16)Magnitude);
		ApplyEffect();
	}
}

void AHtStatusEffect::UpdateDuration(int32 Duration)
{
	if (!bApplied)
	{
		RepSetEffectDuration((uint16)Duration);
		return;
	}


	if (EffectDuration != Duration)
	{
		UnapplyEffect();
		RepSetEffectDuration((uint16)Duration);
		ApplyEffect();
	}
}

void AHtStatusEffect::UpdateMagnitude(int32 Magnitude)
{
	if (!bApplied)
	{
		RepSetEffectMagnitude((uint16)Magnitude);
		return;
	}


	if (EffectMagnitude != Magnitude)
	{
		UnapplyEffect();
		RepSetEffectMagnitude((uint16)Magnitude);
		ApplyEffect();
	}
}

void AHtStatusEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AHtStatusEffect, bApplied, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(AHtStatusEffect, EffectDuration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(AHtStatusEffect, EffectMagnitude, Params);
}
