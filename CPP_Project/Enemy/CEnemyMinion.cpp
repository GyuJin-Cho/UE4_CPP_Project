#include "CEnemyMinion.h"
#include "Global.h"
#include "CEnemyHelath.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Player/CMainPayer.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Action/IWeapon.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Particles/ParticleSystemComponent.h"

ACEnemyMinion::ACEnemyMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &HealthWidget, "HealthWidget", GetMesh());
	CHelpers::CreateComponent(this, &CombatCollision, "CombatCollision", GetCapsuleComponent());
	CHelpers::CreateComponentSocket(this, &AttackLeftCollision, "AttackLeftCollision", "LeftEnemySocket", GetMesh());
	CHelpers::CreateComponentSocket(this, &AttackRightCollision, "AttackRightCollision", "RightEnemySocket", GetMesh());

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'");
	GetMesh()->SetSkeletalMesh(mesh);

	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Status, "Status");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/ABP_EnemyMinion.ABP_EnemyMinion_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	CHelpers::GetClass<UCEnemyHelath>(&SelectWidgetClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_EnemyMinionHealthBar.WB_EnemyMinionHealthBar_C'");
	HealthWidget->SetWidgetClass(SelectWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	//Montage
	CHelpers::GetAsset<UAnimMontage>(&MontageAttack, "AnimMontage'/Game/Enemy/EnemyMinionMontage/Attack_A_Montage.Attack_A_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageHit, "AnimMontage'/Game/Enemy/EnemyMinionMontage/HitReact_Front_Montage.HitReact_Front_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageDead, "AnimMontage'/Game/Enemy/EnemyMinionMontage/Death_A_Montage.Death_A_Montage'");
}
void ACEnemyMinion::BeginPlay()
{
	Super::BeginPlay();

	HealthWidget->SetVisibility(false);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemyMinion::OnBeginOverlapCombat);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ACEnemyMinion::OnEndOverlapCombat);

	AttackLeftCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemyMinion::OnBeginOverlapAttack);
	AttackLeftCollision->OnComponentEndOverlap.AddDynamic(this, &ACEnemyMinion::OnEndOverlapAttack);

	AttackRightCollision->OnComponentBeginOverlap.AddDynamic(this, &ACEnemyMinion::OnBeginOverlapAttack);
	AttackRightCollision->OnComponentEndOverlap.AddDynamic(this, &ACEnemyMinion::OnEndOverlapAttack);

	HealthWidget->InitWidget();
	Cast<UCEnemyHelath>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	AttackLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACEnemyMinion::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	Status->SubHealth(damage);
	if (HitSound && !State->IsDeadMode())
		UGameplayStatics::PlaySound2D(this, HitSound);

	if (Status->GetHealth() <= 0.0f)
	{
		return 0.0f;
	}

	return Status->GetHealth();
}

void ACEnemyMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemyMinion::AttackEnd()
{
	State->SetIdleMode();
	if (bCombatCollisionSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemyMinion::Attack, AttackTime);
	}
}

void ACEnemyMinion::HittedEnd()
{
	State->SetIdleMode();
	if (bCombatCollisionSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemyMinion::Attack, AttackTime);
	}
}

void ACEnemyMinion::CollisionLeftHandOn()
{
	AttackLeftCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACEnemyMinion::CollisionLeftHandOff()
{
	AttackLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACEnemyMinion::CollisionRightHandOn()
{
	AttackRightCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACEnemyMinion::CollisionRightHandOff()
{
	AttackRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACEnemyMinion::DeadAffter()
{
	Destroy();
}

void ACEnemyMinion::OnBeginOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACMainPayer>(OtherActor))
	{
		bCombatCollisionSphere = true;
		HealthWidget->SetVisibility(true);
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &ACEnemyMinion::Attack, AttackTime);
	}

	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass()) && !State->IsDeadMode())
	{
		State->SetHittedMode();
		Hit();
	}
}

void ACEnemyMinion::OnEndOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACMainPayer>(OtherActor))
	{
		bCombatCollisionSphere = false;
		HealthWidget->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(AttackTimer);

	}
}

void ACEnemyMinion::OnBeginOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	if (OtherActor == this)
		return;

	if (Cast<ACMainPayer>(OtherActor))
	{
		if (playerHittedCheck.Num() > 0)
			return;
		ACMainPayer* player = Cast<ACMainPayer>(OtherActor);
		playerHittedCheck.Add(player);
		player->Hitted(Power);
	}
}

void ACEnemyMinion::OnEndOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void ACEnemyMinion::Attack()
{
	State->SetAttackMode();
	if (bCombatCollisionSphere)
	{
		PlayAnimMontage(MontageAttack);
	}
}

void ACEnemyMinion::Hit()
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

	if (State->IsHittedMode() && !State->IsDeadMode())
	{

		PlayAnimMontage(MontageHit);
	}
}

void ACEnemyMinion::Dead()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackLeftCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackRightCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayAnimMontage(MontageDead);
}


