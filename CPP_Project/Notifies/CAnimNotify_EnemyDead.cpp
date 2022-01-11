#include "CAnimNotify_EnemyDead.h"
#include "Global.h"
#include "Enemy/CEnemy.h"
#include "Kismet/GameplayStatics.h"
FString UCAnimNotify_EnemyDead::GetNotifyName_Implementation() const
{
	return "EnemyDead";
}

void UCAnimNotify_EnemyDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemy* enemy = nullptr;
	if (actor)
		enemy = Cast<ACEnemy>(actor);

	if (enemy)
		enemy->DeadAffter();
}
