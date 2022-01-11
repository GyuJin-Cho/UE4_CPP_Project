#include "CAnimNotify_MinionDead.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
FString UCAnimNotify_MinionDead::GetNotifyName_Implementation() const
{
	return "MinionDead";
}

void UCAnimNotify_MinionDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->DeadAffter();
}