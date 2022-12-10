#include "CAxeWeapon.h"
#include "Global.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/CMainPayer.h"
#include "Enemy/CEnemy.h"
#include "Enemy/CEnemyMinion.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

ACAxeWeapon::ACAxeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Mesh, "Mesh", GetRootComponent());
	CHelpers::CreateComponent(this, &Box, "Box", GetRootComponent());
	CHelpers::CreateComponent(this, &Particle, "Particle", Mesh);

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Particle/P_Beam_Impact_Ice.P_Beam_Impact_Ice'");
	Particle->SetTemplate(particle);
}

void ACAxeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAxeWeapon::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACAxeWeapon::OnEndOverlap);
}

void ACAxeWeapon::Tick(float DeltaTime)
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

void ACAxeWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	//한번 피격된 캐릭터는 충돌 처리에서 제외
	ACEnemy* enemy = Cast<ACEnemy>(OtherActor);
	if (enemy)
	{
		for (int i = 0; i < HittedCharacter.Num(); i++)
		{
			if (HittedCharacter[i] == Cast<ACharacter>(OtherActor))
				return;
		}
		HittedCharacter.Add(Cast<ACharacter>(OtherActor));

		if (enemy->HitParticle)
		{
			const USkeletalMeshSocket* weaponSocket = Mesh->GetSocketByName("WeaponSocket");
			if (weaponSocket)
			{
				FVector socketLocation = weaponSocket->GetSocketLocation(Mesh);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), enemy->HitParticle, socketLocation, FRotator(0.f), true);
			}
		}

		FDamageEvent e;
		OtherActor->TakeDamage(Power, e, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}

	ACEnemyMinion* enemyminion = Cast<ACEnemyMinion>(OtherActor);
	if (enemyminion)
	{
		for (int i = 0; i < HittedCharacter.Num(); i++)
		{
			if (HittedCharacter[i] == Cast<ACharacter>(OtherActor))
				return;
		}
		HittedCharacter.Add(Cast<ACharacter>(OtherActor));

		if (enemyminion->HitParticle)
		{
			const USkeletalMeshSocket* weaponSocket = Mesh->GetSocketByName("WeaponSocket");
			if (weaponSocket)
			{
				FVector socketLocation = weaponSocket->GetSocketLocation(Mesh);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), enemyminion->HitParticle, socketLocation, FRotator(0.f), true);
			}
		}

		FDamageEvent e;
		OtherActor->TakeDamage(Power, e, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}
	
}

void ACAxeWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HittedCharacter.Empty();
}

void ACAxeWeapon::Equip(ACMainPayer* player)
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

			player->AxeMode();

			RightHandSocket->AttachActor(this, player->GetMesh());

		}
		if (EuipSound)
			UGameplayStatics::PlaySound2D(this, EuipSound);

	}
}

void ACAxeWeapon::ActivityCollision()
{
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACAxeWeapon::DeactivateCollision()
{
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAxeWeapon::SetPower(float power)
{
	Power = power;
}
void ACAxeWeapon::DestroyWeapon()
{
	Destroy();
}
