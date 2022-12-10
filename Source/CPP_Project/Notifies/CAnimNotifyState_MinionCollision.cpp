#include "CAnimNotifyState_MinionCollision.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
FString UCAnimNotifyState_MinionCollision::GetNotifyName_Implementation() const
{
	return "CollisionLeft";
}

void UCAnimNotifyState_MinionCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->CollisionLeftHandOn();
}

void UCAnimNotifyState_MinionCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->CollisionLeftHandOff();
}