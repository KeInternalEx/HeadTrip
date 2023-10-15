#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HtDamageReceiverComponent.generated.h"

struct FHtDamagePacket;
struct FHtDamageReplicationPacket;

class FHtDamageComponentProxy;

enum class EHtDamageType : uint8;
enum class EHtDamageFlags : uint8;
enum class EHtDamageApplicationFlags : uint8;

/**
 * Place this component on an actor to opt into the damage system as something that can receive damage.
 */
UCLASS(Abstract, BlueprintType)
class HTCORE_API UHtDamageReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	class FDamageReporter;


public:
	UHtDamageReceiverComponent() {};

protected:
	TSharedPtr<FHtDamageComponentProxy> ComponentProxyPtr;

protected:
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

public:
	/**
	 * Called only on the client by the damage subsystem
	 * Eventually goes on to render damage numbers above the parent actor
	 */
	void ApplyDamageClient(const FHtDamagePacket& InDamagePacket);

	/**
	 * Called only on the server by the damage subsystem
	 * Spits out replication packets that the subsystem will then pass to the HtDamageReplicationComponent after filtering
	 */
	void ApplyDamageServer(const FHtDamagePacket& InDamagePacket, TArray<FHtDamageReplicationPacket>& OutReplicationPackets);


protected:
	/**
	 * Called on the client and server to actually do the damage to the parent actor
	 * Derived classes should override this to actually manipulate the parent actor's health
	 */
	virtual void ApplyDamage(const FHtDamagePacket& InDamagePacket, FDamageReporter& InReporter) {}

public:
	/**
	 * Override this to initialize the damage packet with base stats
	 */
	virtual void InitializeDamagePacket(FHtDamagePacket& InDamagePacket);

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FDamageReceivedDelegate, const TArray<FHtDamageReplicationPacket>&)

public:
	/**
	 * Bind to this delegate to get notified when the actor receives damage from another actor.
	 * This gets fired on the game thread after the damage has been applied.
	 * You cannot change the damage here, but you can make modifications to other gameplay elements based on that damage.
	 *
	 * Damage modifications must be done by the FHtDamageComponentProxy
	 */
	FDamageReceivedDelegate& OnDamageReceived() { return DamageReceivedDelegate; }

protected:
	FDamageReceivedDelegate DamageReceivedDelegate;

#pragma region Blueprint API
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void DisplayDamageNumbers(
		int64 Amount, 
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageType)) int32 DamageType, 
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageFlags)) int32 DamageFlags,
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageApplicationFlags)) int32 DamageApplicationFlags);
	virtual void DisplayDamageNumbers_Implementation(int64 Amount, int32 DamageType, int32 DamageFlags, int32 DamageApplicationFlags) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void DisplayExternalDamageNumbers(
		int64 Amount,
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageType)) int32 DamageType,
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageFlags)) int32 DamageFlags,
		UPARAM(meta = (Bitmask, BitmaskEnum = EHtDamageApplicationFlags)) int32 DamageApplicationFlags);
	virtual void DisplayExternalDamageNumbers_Implementation(int64 Amount, int32 DamageType, int32 DamageFlags, int32 DamageApplicationFlags) {}

#pragma endregion
};

