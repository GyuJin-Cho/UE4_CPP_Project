#include "CBTService_Spider.h"
#include "Global.h"
#include "Player/CMainPayer.h"
#include "Enemy/CAIController.h"
#include "Enemy/CEnemy.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Spider::UCBTService_Spider()
{
	NodeName = "Spider";
}

void UCBTService_Spider::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy* aiPawn = Cast<ACEnemy>(controller->GetPawn());
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