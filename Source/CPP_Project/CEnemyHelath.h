#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyHelath.generated.h"

UCLASS()
class CPP_PROJECT_API UCEnemyHelath : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float InHealth, float InMaxHealth);
};
