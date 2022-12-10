#include "CBTTaskNode_MinionSpeed.h"
#include "Global.h"
#include "Enemy/CAIController_Minion.h"
#include "Enemy/CEnemyMinion.h"

UCBTTaskNode_MinionSpeed::UCBTTaskNode_MinionSpeed()
{
	NodeName = "MinionSpeed";
}

EBTNodeResult::Type UCBTTaskNode_MinionSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController_Minion* controller = Cast<ACAIController_Minion>(OwnerComp.GetOwner());
	ACEnemyMinion* aiPawn = Cast<ACEnemyMinion>(controller->GetPawn());

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(aiPawn);

	status->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}
