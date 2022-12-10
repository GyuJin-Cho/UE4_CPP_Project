#include "CAnimNotifyState_EnemyCollision.h"
#include "Global.h"
#include "Enemy/CEnemy.h"
FString UCAnimNotifyState_EnemyCollision::GetNotifyName_Implementation() const
{
	return"EnemyCollision";
}

void UCAnimNotifyState_EnemyCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemy* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemy>(actor);

	if (Enemy)
		Enemy->CollisionOn();
}

void UCAnimNotifyState_EnemyCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemy* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemy>(actor);

	if (Enemy)
		Enemy->CollisionOff();
}