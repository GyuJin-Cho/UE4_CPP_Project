#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Spider.generated.h"

UCLASS()
class CPP_PROJECT_API UCBTService_Spider : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Spider();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
