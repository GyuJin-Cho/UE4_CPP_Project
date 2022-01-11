#include "CAnimNotify_MinionEndAttack.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
FString UCAnimNotify_MinionEndAttack::GetNotifyName_Implementation() const
{
	return "MinionEndAttack";
}

void UCAnimNotify_MinionEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemyMinion* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemyMinion>(actor);

	if (Enemy)
		Enemy->AttackEnd();
}