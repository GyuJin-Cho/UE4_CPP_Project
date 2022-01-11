#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHealthBar.generated.h"

UCLASS()
class CPP_PROJECT_API UCHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void HealthUpdate(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void MarnaUpdate(float InMarna, float InMaxMarna);

	UFUNCTION(BlueprintImplementableEvent)
		void StaminaUpdate(float InStamina, float InMaxStamina);
};
