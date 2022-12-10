#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "CBTTaskNode_MinionSpeed.generated.h"

UCLASS()
class CPP_PROJECT_API UCBTTaskNode_MinionSpeed : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		ECharacterSpeed Type;
public:
	UCBTTaskNode_MinionSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
