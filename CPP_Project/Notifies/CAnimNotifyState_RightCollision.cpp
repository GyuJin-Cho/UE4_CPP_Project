#include "CAnimNotifyState_RightCollision.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
FString UCAnimNotifyState_RightCollision::GetNotifyName_Implementation() const
{
	return "RightCollision";
}

void UCAnimNotifyState_RightCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->CollisionRightHandOn();
}

void UCAnimNotifyState_RightCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->CollisionRightHandOff();
}