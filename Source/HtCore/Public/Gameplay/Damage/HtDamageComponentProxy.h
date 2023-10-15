#pragma once
#include "CoreMinimal.h"

struct FHtDamagePacket;

class UActorComponent;
class UHtDamageDealerComponent;
class UHtDamageReceiverComponent;
class FHtDamageComponentProxy;


class HTCORE_API FHtDamageComponentProxy : public TSharedFromThis<FHtDamageComponentProxy>, public FGCObject
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FMutatePreDelegate, FHtDamagePacket&)
	DECLARE_MULTICAST_DELEGATE_OneParam(FMutatePostDelegate, FHtDamagePacket&)


public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObject(Component);
	}

protected:
	UActorComponent* Component;

public:
	template <
		class TComponent,
		typename = TEnableIf<
		TIsDerivedFrom<TComponent, UHtDamageDealerComponent>::Value ||
		TIsDerivedFrom<TComponent, UHtDamageReceiverComponent>::Value>>
	TWeakObjectPtr<TComponent> GetComponent() const { return static_cast<TComponent*>(Component); }

public:
	virtual void MutatePacketPre(FHtDamagePacket& InDamagePacket) { MutatePreDelegate.Broadcast(InDamagePacket); }
	virtual void MutatePacketPost(FHtDamagePacket& InDamagePacket) { MutatePostDelegate.Broadcast(InDamagePacket); }

public:
	FMutatePreDelegate& OnMutatePre() { return MutatePreDelegate; }
	FMutatePostDelegate& OnMutatePost() { return MutatePostDelegate; }

protected:
	FMutatePreDelegate MutatePreDelegate;
	FMutatePostDelegate MutatePostDelegate;
};


template <class TComponent>
class HTCORE_API THtDamageComponentProxy : public FHtDamageComponentProxy
{
	static_assert(
		TIsDerivedFrom<TComponent, UHtDamageDealerComponent>::Value || TIsDerivedFrom<TComponent, UHtDamageReceiverComponent>::Value,
		"THtDamageComponentProxy requires template parameter derifed from either UHtDamageDealerComponent or UHtDamageReceiverComponent");

public:
	THtDamageComponentProxy(TComponent* InComponent) :
		Component(InComponent) {}

public:

};

