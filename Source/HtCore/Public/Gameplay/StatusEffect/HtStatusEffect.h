// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HtStatusEffect.generated.h"

class IHtStatusEffectContainer;
class UHtStatusEffectIcon;

/**
 * Base class for status effects
 * Tick event fires at the global status effect frequency AHtStatusEffect::EffectTickFreqHz
 */
UCLASS(Abstract, BlueprintType)
class HTCORE_API AHtStatusEffect : public AActor
{
	GENERATED_BODY()
	
public:
	static constexpr float EffectTickFreqHz = 20.0f;

public:	
	// Sets default values for this actor's properties
	AHtStatusEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 
protected:
	/**
	 * Called on the server only to apply the effect to the parent actor
	 * Override this to implement your effect
	 */
	virtual void ApplyEffect() {};

	/**
	 * Called on the server only to apply the effect to the parent actor
	 * Override this to undo the effect
	 */
	virtual void UnapplyEffect() {};

protected:
	inline void RepSetEffectDuration(uint16 NewValue);
	inline void RepSetEffectMagnitude(uint16 NewValue);

private:
	UPROPERTY(ReplicatedUsing="OnRep_EffectApplied") bool bApplied;

private: // Private state, set during application
	UPROPERTY(ReplicatedUsing="OnRep_EffectDuration") uint16 EffectDuration;  // Can be changed at runtime via a function call. Usually only done on the server.
	UPROPERTY(ReplicatedUsing="OnRep_EffectMagnitude") uint16 EffectMagnitude; // Can be changed at runtime via a function call. Usually only done on the server.

private: // Runtime modification only supported on the client via function calls
	UPROPERTY(EditDefaultsOnly) FText EffectName;
	UPROPERTY(EditDefaultsOnly) FText EffectDescription;

	UPROPERTY(EditDefaultsOnly, Instanced) UHtStatusEffectIcon* EffectIcon;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	UFUNCTION() void OnRep_EffectDuration(uint16 NewDuration);
	UFUNCTION() void OnRep_EffectMagnitude(uint16 NewMagnitude);
	UFUNCTION() void OnRep_EffectApplied(bool bNewApplied);


	/**
	 * Called internally to remove the effect
	 */
	UFUNCTION(NetMulticast, Reliable)
	void RemoveInternal();
	void RemoveInternal_Implementation();

	/**
	 * Called internally to apply the effect with a given duration and magnitude
	 */
	UFUNCTION(NetMulticast, Reliable)
	void ApplyInternal(int32 Duration, int32 Magnitude);
	void ApplyInternal_Implementation(int32 Duration, int32 Magnitude);

protected:
	inline IHtStatusEffectContainer* GetContainer() const;
	inline AActor* GetEffectTarget() const;

#pragma region API
public:
	/**
	 * Called on the client and server with the CDO of another status effect as its parameter
	 * Override this to deny OTHER effects from being applied to the owning actor while your effect is applied
	 * Return true to allow the effect to apply
	 */
	virtual bool CanApplyOtherEffect(const TSubclassOf<AHtStatusEffect>& InOtherStatusEffect) const { return true; }

public:
	/**
	 * Call this function to remove the effect.
	 * Authority only
	 * Can be called from blueprints and C++
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Remove();

	/**
	 * Call this function to apply the effect with a given duration and magnitude
	 * Authority only
	 * Returns false if it could not apply the effect
	 * Can be called from blueprints and C++
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool Apply(int32 Duration, int32 Magnitude);
	
	/**
	 * Call this to update the duration and magnitude while the effect is applied.
	 * Does nothing if the effect isn't currently applied.
	 * Can be called from blueprints and C++
	 * Duration and Magnitude are an unsigned 16 bit integer under the hood
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Update(int32 Duration, int32 Magnitude);
	
	/**
	 * Call this to update only the duration while the effect is applied.
	 * Does nothing if the effect isn't currently applied.
	 * Can be called from blueprints and C++
	 * Duration is an unsigned 16 bit integer under the hood
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UpdateDuration(int32 Duration);


	/**
	 * Call this to update only the magnitude while the effect is applied.
	 * Does nothing if the effect isn't currently applied.
	 * Can be called from blueprints and C++
	 * Magnitude is an unsigned 16 bit integer under the hood
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UpdateMagnitude(int32 Magnitude);

public:
	/**
	 * Implement this function to do any cosmetic things related to the application of the effect
	 * Runs on the server and client, make sure to split on has authority
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Apply Effect Cosmetics"))
	void BpApplyEffectCosmetics();
	virtual void BpApplyEffectCosmetics_Implementation() {}

	/**
	 * Implement this function to do any cosmetic things related to the removal of the effect
	 * Runs on the server and client, make sure to split on has authority
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Remove Effect Cosmetics"))
	void BpRemoveEffectCosmetics();
	virtual void BpRemoveEffectCosmetics_Implementation() {}


	/**
	 * Implement this function by calling RemoveActor on self
	 * Runs on the server only, called after RemoveEffectCosmetics
	 */
	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="Remove Effect"))
	void BpRemoveEffect();
	virtual void BpRemoveEffect_Implementation() {}

	/**
	 * Call this to check if another status effect can be applied
	 * While this status effect is currently applied
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DisplayName="Can Apply Other Effect"))
	bool BpCanApplyOtherEffect(const TSubclassOf<AHtStatusEffect>& OtherStatusEffectClass) const { return CanApplyOtherEffect(OtherStatusEffectClass); }

	/**
	 * Gets the target of this effect.
	 * This is actually just the actor that this effect is attached to (calls GetAttachParentActor)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(DisplayName="Get Effect Target"))
	AActor* BpGetEffectTarget() const { return GetAttachParentActor(); }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure) int32 GetEffectDuration() const { return EffectDuration; }
	UFUNCTION(BlueprintCallable, BlueprintPure) int32 GetEffectMagnitude() const { return EffectMagnitude; }

	UFUNCTION(BlueprintCallable, BlueprintPure) const FText& GetEffectName() const { return EffectName; }

	UFUNCTION(BlueprintCallable, BlueprintPure) const FText& GetEffectDescription() const { return EffectDescription; }
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic) void SetEffectName(const FText& NewValue);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic) void SetEffectDescription(const FText& NewValue);

	UFUNCTION(BlueprintCallable) UHtStatusEffectIcon* GetEffectIcon() { return EffectIcon; }
	UFUNCTION(BlueprintCallable, BlueprintPure) bool IsApplied() const { return bApplied; }
#pragma endregion
};
