#include "CPotion.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/CMainPayer.h"
#include "Enemy/CEnemy.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

ACPotion::ACPotion()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Mesh, "Mesh", GetRootComponent());
	CHelpers::CreateComponent(this, &Sphere, "Sphere", GetRootComponent());
	CHelpers::CreateComponent(this, &Particle, "Particle", Mesh);

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/MultistoryDungeons/Meshes/Props/Potion_01.Potion_01'");
	Mesh->SetStaticMesh(mesh);

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Particle/P_Loot_Idle_Loop1.P_Loot_Idle_Loop1'");
	Particle->SetTemplate(particle);
}

void ACPotion::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACPotion::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACPotion::OnEndOverlap);
}

void ACPotion::Tick(float DeltaTime)
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

void ACPotion::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	
	ACMainPayer* player = Cast<ACMainPayer>(OtherActor);
	if (player)
	{
		if(EuipSound)
			UGameplayStatics::PlaySound2D(this, EuipSound);
		player->Heal();
		Destroy();
	}
}

void ACPotion::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

