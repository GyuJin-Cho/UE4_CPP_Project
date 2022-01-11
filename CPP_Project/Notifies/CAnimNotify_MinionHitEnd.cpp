#include "CAnimNotify_MinionHitEnd.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
FString UCAnimNotify_MinionHitEnd::GetNotifyName_Implementation() const
{
	return "MinionHitEnd";
}

void UCAnimNotify_MinionHitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->HittedEnd();
}