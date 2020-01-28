// Adam Weesner 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.h"
#include "Mortar.generated.h"

UCLASS()
class BATTLETANK_API AMortar : public ATank
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMortar();

private:
	int32 Mortar_StartingHealth = 100;
};
