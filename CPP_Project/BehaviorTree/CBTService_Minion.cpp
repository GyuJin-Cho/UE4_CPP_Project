#include "CBTService_Minion.h"
#include "Global.h"
#include "Player/CMainPayer.h"
#include "Enemy/CAIController_Minion.h"
#include "Enemy/CEnemyMinion.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Minion::UCBTService_Minion()
{
	NodeName = "Minion";
}

void UCBTService_Minion::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController_Minion* controller = Cast<ACAIController_Minion>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemyMinion* aiPawn = Cast<ACEnemyMinion>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACMainPayer* target = behavior->GetTargetPlayer();
	if (target == nullptr)
	{
		behavior->SetWaitMode();
		return;
	}

	float distance = aiPawn->GetDistanceTo(target);

	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}