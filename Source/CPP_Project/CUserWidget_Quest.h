#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Quest.generated.h"

UCLASS()
class CPP_PROJECT_API UCUserWidget_Quest : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void SpiderUpdate(int spiderNum);

	UFUNCTION(BlueprintImplementableEvent)
		void MinionUpdate(int minionNum);

	//UPROPERTY(EditAnywhere)
	//	int SpiderNum = 0;
	//UPROPERTY(EditAnywhere)
	//	int MinionNum = 0;
};
