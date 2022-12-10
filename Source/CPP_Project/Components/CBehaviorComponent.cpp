#include "CBehaviorComponent.h"
#include "Global.h"
#include "Player/CMainPayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBehaviorComponent::UCBehaviorComponent()
{

}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

ACMainPayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACMainPayer>(Blackboard->GetValueAsObject(PlayerKey));
}

void UCBehaviorComponent::ChangeType(EBehaviorType InNewType)
{
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InNewType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InNewType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}