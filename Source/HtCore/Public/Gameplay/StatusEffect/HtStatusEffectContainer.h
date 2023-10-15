// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HtStatusEffectContainer.generated.h"


class AHtStatusEffect;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHtStatusEffectContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface describing something that can contain status effects.
 * See Gameplay/HtBaseCharacter for an example of implementation.
 */
class HTCORE_API IHtStatusEffectContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Returns an array of all status effects.
	 * Status effects may or may not be applied.
	 */
	virtual const TArray<AHtStatusEffect*>& GetAllStatusEffects() const = 0;

	/**
	 * Callable from client and server
	 */
	virtual TArray<AHtStatusEffect*> GetStatusEffects(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) = 0;

	/**
	 * Callable from server only
	 * Spawns the status effect actor and attaches it to the container
	 * Also enables replication on the status effect
	 */
	virtual AHtStatusEffect* CreateStatusEffect(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) = 0;

	/**
	 * Callable from client and server
	 */
	virtual bool CanApplyStatusEffect(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) const;


public: // Blueprint Interface

	/**
	 * Get all status effects contained in the container
	 * Returned pointers should not be stored anywhere
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(DisplayName="Get All Status Effects"))
	TArray<AHtStatusEffect*> BpGetAllStatusEffects() const;
	virtual TArray<AHtStatusEffect*> BpGetAllStatusEffects_Implementation() const;

	/**
	 * Get all status effects of a given type contained in the container
	 * Returned pointers should not be stored anywhere
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Get Status Effects"))
	TArray<AHtStatusEffect*> BpGetStatusEffects(TSubclassOf<AHtStatusEffect> StatusEffectClass);
	virtual TArray<AHtStatusEffect*> BpGetStatusEffects_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass);


	/**
	 * Creates a status effect of a given type in the container
	 * Auto attaches the effect to the container
	 * Returned pointer should not be stored anywhere
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintAuthorityOnly, meta = (DisplayName = "Create Status Effect"))
	AHtStatusEffect* BpCreateStatusEffect(TSubclassOf<AHtStatusEffect> StatusEffectClass);
	virtual AHtStatusEffect* BpCreateStatusEffect_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass);

	/**
	 * Checks if a status effect can be applied to the container
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Can Apply Status Effect"))
	bool BpCanApplyStatusEffect(TSubclassOf<AHtStatusEffect> StatusEffectClass) const;
	virtual bool BpCanApplyStatusEffect_Implementation(TSubclassOf<AHtStatusEffect> StatusEffectClass) const;
};
