// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/StatusEffect/HtStatusEffectContainer.h"

#include "HtBaseCharacter.generated.h"

class AHtStatusEffect;


/**
 * Status Effect Methods:
 *	Pointers to the status effects should not be stored outside of the character object.
 *	Instead wrap the returned result(s) in a weak pointer	
 * 
 */
UCLASS(Abstract, BlueprintType)
class HTCORE_API AHtBaseCharacter : public ACharacter, public IHtStatusEffectContainer
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHtBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region IHtStatusEffectContainer
protected:
	UPROPERTY(Replicated) TArray<AHtStatusEffect*> StatusEffects; // This might be an issue with push replication
	// If a status effect gets garbage collected, how would the clients know?
	// Tbf it probably doesn't matter since if an effect is marked as removed it just takes up a bit of memory sitting around

public:
	/**
	 * Returns an array of all status effects.
	 * Status effects may or may not be applied.
	 */
	virtual const TArray<AHtStatusEffect*>& GetAllStatusEffects() const override { return StatusEffects; }

	/**
	 * Callable from client and server
	 */
	virtual TArray<AHtStatusEffect*> GetStatusEffects(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) override;

	/**
	 * Callable from server only
	 */
	virtual AHtStatusEffect* CreateStatusEffect(const TSubclassOf<AHtStatusEffect>& StatusEffectClass) override;


#pragma region Templated StatusEffectContainer


	/**
	 * Templated version of GetStatusEffects
	 * Callable from client and server
	 */
	template <class TStatusEffect, typename = TEnableIf<TIsDerivedFrom<TStatusEffect, AHtStatusEffect>::Value>>
	TArray<TStatusEffect*> GetStatusEffects()
	{
		TArray<TStatusEffect*> Result;
		for (AHtStatusEffect* Effect : StatusEffects)
		{
			if (Effect->IsA<TStatusEffect>())
			{
				Result.Add(static_cast<TStatusEffect*>(Effect));
			}
		}

		return Result;
	}

	/**
	 * Templated version of CreateStatusEffect
	 * Callable from server only
	 */
	template <class TStatusEffect, typename TEnableIf<TIsDerivedFrom<TStatusEffect, AHtStatusEffect>::Value>>
	TStatusEffect* CreateStatusEffect()
	{
		AActor* StatusEffect = GetWorld()->SpawnActor<TStatusEffect>();
		check(StatusEffect != nullptr);

		StatusEffect->SetReplicates(true);
		StatusEffect->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		MARK_PROPERTY_DIRTY_FROM_NAME(AHtBaseCharacter, StatusEffects, this);
		StatusEffects.Add(StatusEffect);
		return StatusEffect;
	}

#pragma endregion

#pragma endregion



public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};


