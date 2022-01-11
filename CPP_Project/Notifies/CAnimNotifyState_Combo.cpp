#include "CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Player/CMainPayer.h"
FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);


	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->DisableCombo();
}
