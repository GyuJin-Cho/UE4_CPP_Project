#include "CAnimNotify_EnemyEndAttack.h"
#include "Global.h"
#include "Enemy/CEnemy.h"
FString UCAnimNotify_EnemyEndAttack::GetNotifyName_Implementation() const
{
	return "EnemyEndAttack";
}
void UCAnimNotify_EnemyEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemy* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemy>(actor);

	if (Enemy)
		Enemy->AttackEnd();
}