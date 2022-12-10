#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Player/CMainPayer.h"
FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->CollisionOn();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->CollisionOff();
}