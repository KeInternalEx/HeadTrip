#include "Gameplay/Damage/HtDamageReceiverComponent.h"
#include "Gameplay/Damage/HtDamageComponentProxy.h"
#include "Gameplay/Damage/HtDamagePacket.h"
#include "Gameplay/Damage/HtDamageReplicationPacket.h"
#include "Gameplay/Damage/HtDamageReceiverComponent.h"



#pragma region Damage Reporter
class UHtDamageReceiverComponent::FDamageReporter
{
public:
	struct FDamageReport
	{
		int64 Amount;
		EHtDamageType Type;
		EHtDamageFlags Flags;
		EHtDamageApplicationFlags ApplicationFlags;
	};

private: friend class UHtDamageReceiverComponent;
	TArray<FDamageReport, TInlineAllocator<4>> Reports;

public:
	void ReportDamageInflicted(
		int64 InAmount,
		EHtDamageType InDamageType,
		EHtDamageFlags InDamageFlags,
		EHtDamageApplicationFlags InApplicationFlags)
	{
		Reports.Add(
		{
			.Amount = InAmount,
			.Type = InDamageType,
			.Flags = InDamageFlags,
			.ApplicationFlags = InApplicationFlags
		});
	}

};
#pragma endregion










void UHtDamageReceiverComponent::EndPlay(EEndPlayReason::Type EndPlayReason) { ComponentProxyPtr.Reset(); } // Drop the reference count so that we can eventually be garbage collected
void UHtDamageReceiverComponent::InitializeDamagePacket(FHtDamagePacket& InDamagePacket)
{
	InDamagePacket.Receiver = this;
	InDamagePacket.ReceiverProxy = ComponentProxyPtr;
}



void UHtDamageReceiverComponent::ApplyDamageClient(const FHtDamagePacket& InDamagePacket)
{
	FDamageReporter Reporter;
	ApplyDamage(InDamagePacket, Reporter);

	// Render damage numbers
	if (!GetOwner()->HasAuthority()) // This is the damage receiver for our local player, don't render damage numbers if we hurt ourselves
	{
		for (const auto& Report : Reporter.Reports)
		{
			DisplayDamageNumbers(Report.Amount, (int32)Report.Type, (int32)Report.Flags, (int32)Report.ApplicationFlags);
		}
	}
}


void UHtDamageReceiverComponent::ApplyDamageServer(const FHtDamagePacket& InDamagePacket, TArray<FHtDamageReplicationPacket>& OutReplicationPackets)
{
	FDamageReporter Reporter;
	ApplyDamage(InDamagePacket, Reporter);

	for (const auto& Report : Reporter.Reports)
	{
		OutReplicationPackets.Add(
		{
			.DamageTaken = (uint64)Report.Amount,
			.Type = Report.Type,
			.Flags = Report.Flags,
			.ApplicationFlags = Report.ApplicationFlags,
			.Receiver = InDamagePacket.Receiver.Get(),
			.Attacker = InDamagePacket.Attacker.Get(),
		});
	}

	DamageReceivedDelegate.Broadcast(OutReplicationPackets);
}

