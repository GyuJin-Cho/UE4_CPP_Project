#include "CAnimNotify_HittedEnd.h"
#include "Global.h"
#include "Enemy/CEnemy.h"
FString UCAnimNotify_HittedEnd::GetNotifyName_Implementation() const
{
	return "HittedEnd";
}
void UCAnimNotify_HittedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACEnemy* Enemy = nullptr;
	if (actor)
		Enemy = Cast<ACEnemy>(actor);

	if (Enemy)
		Enemy->HittedEnd();
}