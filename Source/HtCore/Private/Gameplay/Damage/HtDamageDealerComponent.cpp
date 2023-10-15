#include "Gameplay/Damage/HtDamageDealerComponent.h"
#include "Gameplay/Damage/HtDamagePacket.h"
#include "Gameplay/Damage/HtDamageReplicationPacket.h"
#include "Gameplay/Damage/HtDamageComponentProxy.h"
#include "Gameplay/Damage/HtDamageReceiverComponent.h"
#include "Gameplay/Damage/HtDamageSubsystem.h"


void UHtDamageDealerComponent::EndPlay(EEndPlayReason::Type EndPlayReason) { ComponentProxyPtr.Reset(); } // Drop the reference count so that we can eventually be garbage collected
void UHtDamageDealerComponent::InitializeDamagePacket(FHtDamagePacket& InDamagePacket)
{
	InDamagePacket.Attacker = this;
	InDamagePacket.AttackerProxy = ComponentProxyPtr;
}

