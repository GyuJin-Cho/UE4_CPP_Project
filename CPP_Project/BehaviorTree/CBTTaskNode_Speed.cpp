#include "CBTTaskNode_Speed.h"
#include "Global.h"
#include "Enemy/CAIController.h"
#include "Enemy/CEnemy.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(aiPawn);

	status->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}