#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

UCLASS()
class CPP_PROJECT_API UCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		bool bStartMenu = false;
};
