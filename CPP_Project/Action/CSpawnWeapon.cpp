#include "CSpawnWeapon.h"
#include "Global.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/CMainPayer.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

ACSpawnWeapon::ACSpawnWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Mesh,"Mesh",GetRootComponent());
	CHelpers::CreateComponent(this, &Box,"Box", GetRootComponent());
	
}

void ACSpawnWeapon::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSpawnWeapon::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSpawnWeapon::OnEndOverlap);
}

void ACSpawnWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Yaw = Mesh->GetRelativeRotation().Yaw + YawRolling;
	if (FMath::IsNearlyEqual(Yaw, 360.0f))
		Yaw = 0;

	if (Rotate)
	{
		Mesh->SetRelativeRotation(FRotator(0, Yaw, 0));
	}
}

void ACSpawnWeapon::Equip(ACMainPayer* player)
{
	if (player)
	{
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Weapon_Handle"));
		
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		
		Mesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = player->GetMesh()->GetSocketByName("Weapon_Handle");
		if (RightHandSocket)
		{
			Rotate = false;

			Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			player->SwordMode();

			RightHandSocket->AttachActor(this, player->GetMesh());
		
		}
		if (EuipSound)
			UGameplayStatics::PlaySound2D(this, EuipSound);
		
	}
}

void ACSpawnWeapon::ActivityCollision()
{
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACSpawnWeapon::DeactivateCollision()
{
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACSpawnWeapon::SetPower(float power)
{
	Power = power;
}

void ACSpawnWeapon::DestroyWeapon()
{
	Destroy();
}

void ACSpawnWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	//한번 피격된 캐릭터는 충돌 처리에서 제외
	if (Cast<ACharacter>(OtherActor))
	{
		for (int i = 0; i < HittedCharacter.Num(); i++)
		{
			if (HittedCharacter[i] == Cast<ACharacter>(OtherActor))
				return;
		}
		HittedCharacter.Add(Cast<ACharacter>(OtherActor));

		FDamageEvent e;
		OtherActor->TakeDamage(Power, e, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}


}

void ACSpawnWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HittedCharacter.Empty();
}
