#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Minion.generated.h"

UCLASS()
class CPP_PROJECT_API UCBTService_Minion : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Minion();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
