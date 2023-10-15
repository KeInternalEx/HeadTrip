#pragma once

#include "CoreMinimal.h"


#include "HtDamageReplicationPacket.generated.h"

enum class EHtDamageType : uint8;
enum class EHtDamageFlags : uint8;
enum class EHtDamageApplicationFlags : uint8;

class UHtDamageReceiverComponent;
class UHtDamageDealerComponent;





USTRUCT()
struct FHtDamageReplicationPacket
{
    GENERATED_BODY()

    UPROPERTY() uint64 DamageTaken;
    UPROPERTY() EHtDamageType Type;
    UPROPERTY() EHtDamageFlags Flags;
    UPROPERTY() EHtDamageApplicationFlags ApplicationFlags;
    UPROPERTY() UHtDamageReceiverComponent* Receiver;
    UPROPERTY() UHtDamageDealerComponent* Attacker;
};

