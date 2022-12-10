#include "CAIController_Minion.h"
#include "Global.h"
#include "CEnemyMinion.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/CMainPayer.h"

ACAIController_Minion::ACAIController_Minion()
{
	CHelpers::CreateActorComponent(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 180.0f;
	Sight->SetMaxAge(2.0f);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController_Minion::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController_Minion::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemyMinion>(InPawn);
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	SetGenericTeamId(OwnerEnemy->GetTeamID());
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Minion::OnPerceptionUpdated);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

	Behavior->SetBlackboard(Blackboard);
}

void ACAIController_Minion::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

float ACAIController_Minion::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController_Minion::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	ACMainPayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACMainPayer>(actor);

		if (!!player)
			break;
	}

	Blackboard->SetValueAsObject("Player", player);
}
