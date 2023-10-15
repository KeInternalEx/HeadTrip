#pragma once

#include "CoreMinimal.h"

class FHtDamageComponentProxy;
class UHtDamageDealerComponent;
class UHtDamageReceiverComponent;

enum class EHtDamageType : uint8;
enum class EHtDamageFlags : uint8;

struct FHtDamagePacket
{
public:
	TSharedPtr<FHtDamageComponentProxy, ESPMode::ThreadSafe> AttackerProxy;
	TSharedPtr<FHtDamageComponentProxy, ESPMode::ThreadSafe> ReceiverProxy;

	TWeakObjectPtr<UHtDamageDealerComponent> Attacker;
	TWeakObjectPtr<UHtDamageReceiverComponent> Receiver;

public:
	FVector NetRelevancyLocation;

public:
	uint16 BaseDamage;

public:
	EHtDamageType Type;
	EHtDamageFlags Flags;

public:
	uint16 CritChance;
	uint16 BlockChance;

public:
	float CritMultiplier;
	float BlockMultiplier;
	float LocationalMultiplier;

public:
	uint64 CalculatedDamage;
	uint8  CritTier;
	uint8  BlockTier;
};


