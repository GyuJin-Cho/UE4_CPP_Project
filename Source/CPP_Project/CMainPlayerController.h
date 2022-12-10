#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CMainPlayerController.generated.h"

UCLASS()
class CPP_PROJECT_API ACMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void ShowCursor();
	void OffShowCursor();
};
