#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"


#include "HtDamageDealerComponent.generated.h"


struct FHtDamagePacket;
struct FHtDamageReplicationPacket;

class FHtDamageComponentProxy;
class UHtDamageReceiverComponent;


/**
 * Place this component on an actor to opt into the damage system as something that can deal damage.
 */
UCLASS(Abstract, BlueprintType)
class HTCORE_API UHtDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHtDamageDealerComponent() {};

protected:
	TSharedPtr<FHtDamageComponentProxy> ComponentProxyPtr;



protected:
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

public: // Native interface

	virtual void InitializeDamagePacket(FHtDamagePacket& InDamagePacket);
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDamageInflictedDelegate, const TArray<FHtDamageReplicationPacket>&)

public:
	/**
	 * Bind to this delegate to get notified when the actor does damage to another actor.
	 * This gets fired on the game thread after the damage has been applied.
	 * You cannot change the damage here.
	 * 
	 * Damage modifications must be done by the FHtDamageComponentProxy
	 */
	FDamageInflictedDelegate& OnDamageInflicted() { return DamageInflictedDelegate; }

protected: friend class UHtDamageSubsystem;
	FDamageInflictedDelegate DamageInflictedDelegate;
};

