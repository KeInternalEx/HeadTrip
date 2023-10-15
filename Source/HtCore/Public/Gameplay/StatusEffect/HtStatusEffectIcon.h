// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HtStatusEffectIcon.generated.h"

class AHtStatusEffect;


/**
 * Implements an icon for a widget, automatically ticks down duration
 * While the status effect is applied
 */
UCLASS(Abstract, BlueprintType)
class HTCORE_API UHtStatusEffectIcon : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY() AHtStatusEffect* StatusEffect;

public:
	inline void SetStatusEffect(AHtStatusEffect* InStatusEffect) { StatusEffect = InStatusEffect; }
	inline AHtStatusEffect* GetStatusEffect() const { return StatusEffect; }

#pragma region Blueprint API
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, BlueprintPure, meta=(DisplayName="Get Status Effect"))
	AHtStatusEffect* BpGetStatusEffect() const { return GetStatusEffect(); }

public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Duration Changed"))
	void BpDurationChanged(int32 OldDuration, int32 NewDuration);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Magnitude Changed"))
	void BpMagnitudeChanged(int32 OldMagnitude, int32 NewMagnitude);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Name Changed"))
	void BpNameChanged(const FText& OldName, const FText& NewName);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Description Changed"))
	void BpDescriptionChanged(const FText& OldDescription, const FText& NewDescription);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Applied"))
	void BpApplied();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Removed"))
	void BpRemoved();

#pragma endregion




};
