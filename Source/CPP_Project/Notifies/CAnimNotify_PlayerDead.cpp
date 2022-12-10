#include "CAnimNotify_PlayerDead.h"
#include "Global.h"
#include "Player/CMainPayer.h"
#include "Kismet/GameplayStatics.h"

FString UCAnimNotify_PlayerDead::GetNotifyName_Implementation() const
{
	return "PlayerDead";
}
void UCAnimNotify_PlayerDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	AActor* actor = MeshComp->GetOwner();
	ACMainPayer* player = nullptr;
	if (actor)
		player = Cast<ACMainPayer>(actor);

	if (player)
		player->DeadAffter();
}