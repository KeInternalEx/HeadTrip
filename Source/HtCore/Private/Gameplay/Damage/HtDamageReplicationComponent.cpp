#include "Gameplay/Damage/HtDamageReplicationComponent.h"
#include "Gameplay/Damage/HtDamageReceiverComponent.h"
#include "Gameplay/Damage/HtDamageReplicationPacket.h"



void UHtDamageReplicationComponent::ReplicateLocalDamageTaken_Implementation(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets)
{
	LocalPlayerDamagedDelegate.Broadcast(InDamageReplicationPackets);
}

void UHtDamageReplicationComponent::ReplicateExternalDamageNumberDisplay_Implementation(const TArray<FHtDamageReplicationPacket>& InDamageReplicationPackets)
{
	ExternalDamageNumbersReceivedDelegate.Broadcast(InDamageReplicationPackets);
	
	for (const FHtDamageReplicationPacket& ReplicationPacket : InDamageReplicationPackets)
    {
        ReplicationPacket.Receiver->DisplayExternalDamageNumbers(
            ReplicationPacket.DamageTaken,
            (int32)ReplicationPacket.Type,
            (int32)ReplicationPacket.Flags,
            (int32)ReplicationPacket.ApplicationFlags);
	}
}
