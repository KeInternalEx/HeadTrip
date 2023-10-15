#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EHtDamageType : uint8
{
	DamageType_None			= 0 << 0 UMETA(Hidden),
	DamageType_Fire			= 1 << 0,
	DamageType_Water		= 1 << 1,
	DamageType_Air			= 1 << 2,
	DamageType_Earth		= 1 << 3,
};

ENUM_CLASS_FLAGS(EHtDamageType);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EHtDamageFlags : uint8
{
	DamageFlags_None					= 0 << 0,
	DamageFlags_IgnoreResistance		= 1 << 0,
	DamageFlags_IgnoreBlock				= 1 << 1,
	DamageFlags_IgnoreArmor				= 1 << 2,
	DamageFlags_IgnoreHealth			= 1 << 3,
	DamageFlags_IgnoreLocationalDamage	= 1 << 4,
	DamageFlags_IgnoreCrit				= 1 << 5,
	DamageFlags_IgnoreEffects			= 1 << 6
};

ENUM_CLASS_FLAGS(EHtDamageFlags);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EHtDamageApplicationFlags : uint8
{
	DamageApplicationFlags_None = 0 << 0 UMETA(Hidden),
	DamageApplicationFlags_Health = 0 << 1,
	DamageApplicationFlags_Armor = 0 << 2
};

ENUM_CLASS_FLAGS(EHtDamageApplicationFlags);


