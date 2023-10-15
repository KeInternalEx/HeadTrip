#include "Gameplay/Player/HtPlayerCharacter.h"


AHtPlayerCharacter::AHtPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHtPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AHtPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHtPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

