#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Player/CMainPayer.h"
FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->Nextcombo();
}
