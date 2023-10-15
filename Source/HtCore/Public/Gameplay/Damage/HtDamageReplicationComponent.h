#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HtDamageReplicationComponent.generated.h"


struct FHtDamageReplicationPacket;

/**
 * This component should be placed on the player controller.
 * Responsible for replicating client bound RPCs to players to tell them about damage events.
 */
UCLASS()
class UHtDamageReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FLocalPlayerDamagedDelegate, const TArray<FHtDamageReplicationPacket>&)
	DECLARE_MULTICAST_DELEGATE_OneParam(FExternalDamageNumbersReceivedDelegate, const TArray<FHtDamageReplicationPacket>&)


public:
	UHtDamageReplicationComponent() {}

public:
	/**
	 * Reliable client RPC telling a specific client that they took damage
	 */
	UFUNCTION(Client, Reliable)
	void ReplicateLocalDamageTaken(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets);
	void ReplicateLocalDamageTaken_Implementation(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets);

	/**
	 * Unreliable client RPC telling a specific client to display damage numbers over an actor
	 * Respects net relevancy rules
	 */
	UFUNCTION(Client, Unreliable)
	void ReplicateExternalDamageNumberDisplay(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets);
	void ReplicateExternalDamageNumberDisplay_Implementation(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets);

public:
	FLocalPlayerDamagedDelegate& OnLocalPlayerDamagedDelegate() { return LocalPlayerDamagedDelegate; }
	FExternalDamageNumbersReceivedDelegate& OnExternalDamageNumbersReceived() { return ExternalDamageNumbersReceivedDelegate; }

private:
	FLocalPlayerDamagedDelegate LocalPlayerDamagedDelegate;
	FExternalDamageNumbersReceivedDelegate ExternalDamageNumbersReceivedDelegate;
};

