// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Damage/HtDamageSubsystem.h"
#include "Gameplay/Damage/HtDamagePacket.h"
#include "Gameplay/Damage/HtDamageReplicationPacket.h"
#include "Gameplay/Damage/HtDamageComponentProxy.h"
#include "Gameplay/Damage/HtDamageDealerComponent.h"
#include "Gameplay/Damage/HtDamageReceiverComponent.h"
#include "Gameplay/Damage/HtDamageReplicationComponent.h"


#include "EngineUtils.h" 
#include "HAL/Runnable.h"



#pragma region Async Packet Processor
class UHtDamageSubsystem::FHtDamageProcessorRunnable final : public FRunnable
{
private: // Thread safe queues
	TQueue<FHtDamagePacket, EQueueMode::Spsc> PendingQueue; // Written to by the game thread, read by the async thread
	TQueue<FHtDamagePacket, EQueueMode::Spsc> CompletedQueue; // Written to by the async thread, read by the game thread

public:
	bool EnqueuePendingPacket(const FHtDamagePacket& InDamagePacket)
	{
		check(IsInGameThread());
		return PendingQueue.Enqueue(InDamagePacket);
	}

	bool DequeueCompletedPacket(FHtDamagePacket& OutDamagePacket)
	{
		check(IsInGameThread());
		return CompletedQueue.Dequeue(OutDamagePacket);
	}

public:
	uint32 Run() override
	{
		FHtDamagePacket CurrentPacket;
		while (bShouldRun)
		{
			if (PendingQueue.Dequeue(CurrentPacket))
			{
				if (!CurrentPacket.AttackerProxy.IsValid() ||
					!CurrentPacket.ReceiverProxy.IsValid())
				{
					continue; // Orphaned packet, get rid of it
				}

				CurrentPacket.AttackerProxy->MutatePacketPre(CurrentPacket);
				CurrentPacket.ReceiverProxy->MutatePacketPre(CurrentPacket);
				CurrentPacket.AttackerProxy->MutatePacketPost(CurrentPacket);
				CurrentPacket.ReceiverProxy->MutatePacketPost(CurrentPacket);

				CompletedQueue.Enqueue(CurrentPacket);
			}
		}

		return 0;
	}

private:
	bool bShouldRun;
	FRunnableThread* Thread;

public:
	void Stop() override { bShouldRun = false; }

public:
	FHtDamageProcessorRunnable() :
		bShouldRun(true)
	{
		Thread = FRunnableThread::Create(this, TEXT("HtDamageSubsystemThread"));
	}

	~FHtDamageProcessorRunnable()
	{
		if (Thread != nullptr)
		{
			Thread->Kill();
			delete Thread;
		}
	}
};
#pragma endregion






void UHtDamageSubsystem::Initialize(FSubsystemCollectionBase& Collection) { Runnable = new FHtDamageProcessorRunnable(); }
void UHtDamageSubsystem::Deinitialize() { delete Runnable; }


void UHtDamageSubsystem::Tick(float DeltaTime)
{
	uint16 PacketsProcessed = 0;
	FHtDamagePacket DamagePacket;

	while (
		(++PacketsProcessed < MaxDamagePacketsProcessedPerFrame) &&
		(Runnable->DequeueCompletedPacket(DamagePacket)))
	{
		if (!DamagePacket.Attacker.IsValid() ||
			!DamagePacket.Receiver.IsValid()) // Components for this packet have been destroyed, skip this one
		{
			continue;
		}


#ifdef WITH_SERVER_CODE
		TArray<FHtDamageReplicationPacket> ReplicationPackets;

		DamagePacket.Receiver->ApplyDamageServer(DamagePacket, ReplicationPackets); // Do the damage and create replication packets
		DamagePacket.Attacker->DamageInflictedDelegate.Broadcast(ReplicationPackets);

		APlayerController* AttackerController = DamagePacket.Attacker->GetTypedOuter<APlayerController>();
		APlayerController* ReceiverController = DamagePacket.Receiver->GetTypedOuter<APlayerController>();

		if (AttackerController != nullptr) // Attacker is a player, search for players that are subscribed to external damage render events
		{
			for (TObjectIterator<UHtDamageReplicationComponent> RepComponentIterator; RepComponentIterator; ++RepComponentIterator)
			{
				if (RepComponentIterator->GetWorld() == GetWorld())
				{

					APlayerController* PlayerController = static_cast<APlayerController*>(RepComponentIterator->GetOwner());
					if (PlayerController == AttackerController) // Don't bother replicating to the instigator
						continue;

					// TODO: CHECK IF THIS ACTOR HAS OPTED IN
					RepComponentIterator->ReplicateExternalDamageNumberDisplay(ReplicationPackets);
				}
			}
		}

		if (ReceiverController != nullptr) // Receiver is a player, replicate damage taken to them
		{
			ReceiverController
				->GetComponentByClass<UHtDamageReplicationComponent>()
				->ReplicateLocalDamageTaken(ReplicationPackets);
		}

#else
		DamagePacket.Receiver->ApplyDamageClient(DamagePacket);
#endif

		
	}
}

