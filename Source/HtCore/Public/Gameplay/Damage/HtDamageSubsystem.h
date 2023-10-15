// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HtDamageSubsystem.generated.h"


/**
 * 
 */
UCLASS(Config = "Engine")
class HTCORE_API UHtDamageSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
private:
	class FHtDamageProcessorRunnable;
	FHtDamageProcessorRunnable* Runnable;

private:
	UPROPERTY(Config = "Engine") uint16 MaxDamagePacketsProcessedPerFrame;


protected:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

protected:
	void Tick(float DeltaTime) override;
};
