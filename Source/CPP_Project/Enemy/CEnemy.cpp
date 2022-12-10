#include "CEnemy.h"
#include "Global.h"
#include "CEnemyHelath.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Player/CMainPayer.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Action/IWeapon.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &HealthWidget, "HealthWidget", GetMesh());
	CHelpers::CreateComponent(this, &CombatCollision, "CombatCollision", GetCapsuleComponent());
	CHelpers::CreateComponentSocket(this, &AttackCollision, "AttackCollision", "EnemySocket", GetMesh());
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider'");
	GetMesh()->SetSkeletalMesh(mesh);

	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Status, "Status");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/ABP_Enemy.ABP_Enemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Particle/PS_Blood_Splatter.PS_Blood_Splatter'");
	HitParticle = particle;

	CHelpers::GetClass<UCEnemyHelath>(&SelectWidgetClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_EnemyHealthBar.WB_EnemyHealthBar_C'");
	HealthWidget->SetWidgetClass(SelectWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	CHelpers::GetAsset<UAnimMontage>(&MontageAttack, "AnimMontage'/Game/Enemy/Montage/ExoGame_Greater_Spider_Attack_Melee_B_Montage.ExoGame_Greater_Spider_Attack_Melee_B_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageHit, "AnimMontage'/Game/Enemy/Montage/ExoGame_Greater_Spider_React_Heavy_Front_Montage.ExoGame_Greater_Spider_React_Heavy_Front_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageDead, "AnimMontage'/Game/Enemy/Montage/ExoGame_Greater_Spider_Death_Montage.ExoGame_Greater_Spider_Death_Montage'");

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthWidget->SetVisibility(false);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::OnBeginOverlapCombat);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ACEnemy::OnEndOverlapCombat);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::OnBeginOverlapAttack);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &ACEnemy::OnEndOverlapAttack);

	HealthWidget->InitWidget();
	Cast<UCEnemyHelath>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void ACEnemy::OnBeginOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<ACMainPayer>(OtherActor))
	{
		bCombatCollisionSphere = true;
		HealthWidget->SetVisibility(true);
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemy::Attack, AttackTime);
	}

	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass())&&!State->IsDeadMode())
	{
		State->SetHittedMode();
		Hit();
	}
}

void ACEnemy::OnEndOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACMainPayer>(OtherActor))
	{
		bCombatCollisionSphere = false;
		HealthWidget->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(AttackTimer);
		
	}

}

void ACEnemy::OnBeginOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	if (OtherActor == this)
		return;
	
	if (Cast<ACMainPayer>(OtherActor))
	{
		if (playerHittedCheck.Num() > 0)
			return;
		ACMainPayer* player = Cast<ACMainPayer>(OtherActor);
		if (player)
		{
			playerHittedCheck.Add(player);

			const USkeletalMeshSocket* tipSocket = GetMesh()->GetSocketByName("TipSocket");
			if (tipSocket)
			{
				FVector socketLocation = tipSocket->GetSocketLocation(GetMesh());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->HitParticle, socketLocation, FRotator(0.0f), false);
			}

			player->Hitted(Power);
		}
	}
}

void ACEnemy::OnEndOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	if (OtherActor == this)
		return;

	if (Cast<ACMainPayer>(OtherActor))
	{
		ACMainPayer* player = Cast<ACMainPayer>(OtherActor);
		playerHittedCheck.Empty();
		player->HittedEnd();
	}
}

void ACEnemy::Attack()
{
	State->SetAttackMode();
	if (bCombatCollisionSphere)
	{
		PlayAnimMontage(MontageAttack);
	}
}

void ACEnemy::Hit()
{
	Cast<UCEnemyHelath>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	Status->SetMove();

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		Dead();
	}

	if (State->IsHittedMode()&&!State->IsDeadMode())
	{

		PlayAnimMontage(MontageHit);
	}
}

void ACEnemy::Dead()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PlayAnimMontage(MontageDead);
	ACMainPayer* player = Cast<ACMainPayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->SubSpider();
	
}

void ACEnemy::DeadAffter()
{
	Destroy();
}

void ACEnemy::AttackEnd()
{
	State->SetIdleMode();
	if (bCombatCollisionSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemy::Attack, AttackTime);
	}
}

void ACEnemy::HittedEnd()
{
	State->SetIdleMode();
	if (bCombatCollisionSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemy::Attack, AttackTime);
	}
}

void ACEnemy::CollisionOn()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACEnemy::CollisionOff()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	Status->SubHealth(damage);
	if (HitSound&&!State->IsDeadMode())
		UGameplayStatics::PlaySound2D(this, HitSound);

	if (Status->GetHealth() <= 0.0f)
	{
		return 0.0f;
	}
	
	return Status->GetHealth();
}