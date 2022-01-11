#include "CAnimNotify_EndAttack.h"
#include "Global.h"
#include "Player/CMainPayer.h"
#include "Kismet/GameplayStatics.h"
FString UCAnimNotify_EndAttack::GetNotifyName_Implementation() const
{
	return "EndAttack";
}

void UCAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if(player)
		player->End_bAttackMode();
}
