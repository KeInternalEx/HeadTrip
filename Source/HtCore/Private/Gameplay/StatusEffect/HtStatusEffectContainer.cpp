// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/StatusEffect/HtStatusEffectContainer.h"
#include "Gameplay/StatusEffect/HtStatusEffect.h"


// Add default functionality here for any IHtStatusEffectContainer functions that are not pure virtual.


bool IHtStatusEffectContainer::CanApplyStatusEffect(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) const
{
	for (const AHtStatusEffect* StatusEffect : GetAllStatusEffects())
	{
		if (StatusEffect->IsApplied() && !StatusEffect->CanApplyOtherEffect(StatusEffectClass))
		{
			return false;
		}
	}

	return true;
}

TArray<AHtStatusEffect*> IHtStatusEffectContainer::BpGetAllStatusEffects_Implementation() const { return GetAllStatusEffects(); }
TArray<AHtStatusEffect*> IHtStatusEffectContainer::BpGetStatusEffects_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass) { return GetStatusEffects(StatusEffectClass); }
AHtStatusEffect* IHtStatusEffectContainer::BpCreateStatusEffect_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass) { return CreateStatusEffect(StatusEffectClass); }
bool IHtStatusEffectContainer::BpCanApplyStatusEffect_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass) const { return CanApplyStatusEffect(StatusEffectClass); }
