#pragma once

#include "CoreMinimal.h"
#include "Gameplay/HtBaseCharacter.h"

#include "HtPlayerCharacter.generated.h"


UCLASS()
class HTCORE_API AHtPlayerCharacter : public AHtBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values
	AHtPlayerCharacter();


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// Status Effects
};


