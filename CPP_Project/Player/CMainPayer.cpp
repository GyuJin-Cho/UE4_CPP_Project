#include "CMainPayer.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "CHealthBar.h"
#include "CUserWidget_SkillSets.h"
#include "CUserWidget_AxeSklii.h"
#include "CUserWidget_Quest.h"
#include "Blueprint/UserWidget.h"
#include "Action/IWeapon.h"
#include "Action/CSpawnWeapon.h"
#include "Action/CAxeWeapon.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "CMainPlayerController.h"
#include "CStartMenu.h"
#include "Save/CSaveGame.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

ACMainPayer::ACMainPayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_PlayerAninInstance.ABP_PlayerAninInstance_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	bUseControllerRotationYaw = false;

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Particle/PS_Blood_Splatter.PS_Blood_Splatter'");
	HitParticle = particle;

	//Montage
	CHelpers::GetAsset<UAnimMontage>(&MontageSword[0], "AnimMontage'/Game/Chracter/Montage/Attack1_Montage.Attack1_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageSword[1], "AnimMontage'/Game/Chracter/Montage/Attack2_Montage.Attack2_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageSword[2], "AnimMontage'/Game/Chracter/Montage/Attack3_Montage.Attack3_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageAxe[0], "AnimMontage'/Game/Chracter/Montage/AxeAttack1_Montage.AxeAttack1_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageAxe[1], "AnimMontage'/Game/Chracter/Montage/AxeAttack2_Montage.AxeAttack2_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&MontageDead, "AnimMontage'/Game/Chracter/Montage/Death_Montage.Death_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&QSkill[0], "AnimMontage'/Game/Chracter/Montage/StrongAttack.StrongAttack'");
	CHelpers::GetAsset<UAnimMontage>(&ESkill[0], "AnimMontage'/Game/Chracter/Montage/DrawAttack.DrawAttack'");
	CHelpers::GetAsset<UAnimMontage>(&QSkill[1], "AnimMontage'/Game/Chracter/Montage/AxeQSkill.AxeQSkill'");
	CHelpers::GetAsset<UAnimMontage>(&ESkill[1], "AnimMontage'/Game/Chracter/Montage/AxeEskill.AxeEskill'");

	//Widget
	CHelpers::GetClass<UCHealthBar>(&SelectWidgetClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_HealthBar.WB_HealthBar_C'");
	CHelpers::GetClass<UCStartMenu>(&StartLevelWidgetClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_StartLevel.WB_StartLevel_C'");
	CHelpers::GetClass<UCUserWidget_SkillSets>(&SkillsetsClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_SkillSets.WB_SkillSets_C'");
	CHelpers::GetClass<UCUserWidget_AxeSklii>(&AxeSkillsetsClass, "	WidgetBlueprint'/Game/Widgets/Widgets/WB_AxeSkillSet.WB_AxeSkillSet_C'");
	CHelpers::GetClass<UCUserWidget_Quest>(&QuestClass, "WidgetBlueprint'/Game/Widgets/Widgets/WB_Quest.WB_Quest_C'");
	StaminaDrainRate = 0.05f;

}

void ACMainPayer::BeginPlay()
{
	Super::BeginPlay();
	
	HelthBarWidget = CreateWidget<UCHealthBar, APlayerController>(GetController<APlayerController>(), SelectWidgetClass);
	HelthBarWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
	HelthBarWidget->MarnaUpdate(Status->GetMarna(), Status->GetMaxMarna());
	HelthBarWidget->StaminaUpdate(Status->GetStamina(), Status->GetMaxStamina());

	StartLevelWidget = CreateWidget<UCStartMenu>(GetWorld(), StartLevelWidgetClass);
	SkillsetsWidget = CreateWidget<UCUserWidget_SkillSets>(GetWorld(), SkillsetsClass);
	AxeSkillsetsWidget = CreateWidget<UCUserWidget_AxeSklii>(GetWorld(), AxeSkillsetsClass);
	QuestWidget = CreateWidget<UCUserWidget_Quest>(GetWorld(), QuestClass);
	QuestWidget->MinionUpdate(MinionCount);
	QuestWidget->SpiderUpdate(SpiderCount);

	Load();
	if (!bShow)
	{
		StartLevelWidget->AddToViewport();
		StartLevelWidget->SetVisibility(ESlateVisibility::Visible);
		HelthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HelthBarWidget->AddToViewport();
		QuestWidget->AddToViewport();
		HelthBarWidget->SetVisibility(ESlateVisibility::Visible);
		QuestWidget->SetVisibility(ESlateVisibility::Visible);
		StartLevelWidget->SetVisibility(ESlateVisibility::Hidden);
		Save();
	}
//AnimMontage'/Game/Chracter/Montage/AxeEskill.AxeEskill'	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACMainPayer::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACMainPayer::OnEndOverlap);
}

void ACMainPayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime;
	if (bRun)
	{
		if(Status->GetStamina()<0)
			GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
		Status->SubStamina(StaminaDrainRate);
		HelthBarWidget->StaminaUpdate(Status->GetStamina(), Status->GetMaxStamina());
	}
	else
	{
		Status->AddStamina(StaminaDrainRate);
		HelthBarWidget->StaminaUpdate(Status->GetStamina(), Status->GetMaxStamina());
	}
}

void ACMainPayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACMainPayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACMainPayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACMainPayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACMainPayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, this, &ACMainPayer::Jump);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Released, this, &ACMainPayer::JumpEnd);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACMainPayer::Run);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACMainPayer::RunEnd);
	PlayerInputComponent->BindAction("EquipItem", EInputEvent::IE_Released, this, &ACMainPayer::Equip);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &ACMainPayer::Attack);
	PlayerInputComponent->BindAction("QSkill", EInputEvent::IE_Pressed, this, &ACMainPayer::QSkillAttack);
	PlayerInputComponent->BindAction("Eskill", EInputEvent::IE_Pressed, this, &ACMainPayer::ESkillAttack);
}

FGenericTeamId ACMainPayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACMainPayer::OnMoveForward(float InAxis)
{
	if (Status->CanMove())
	{
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector direction = FQuat(rotator).GetForwardVector();

		AddMovementInput(direction, InAxis);
	}

}

void ACMainPayer::OnMoveRight(float InAxis)
{
	if (Status->CanMove())
	{
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector direction = FQuat(rotator).GetRightVector();

		AddMovementInput(direction, InAxis);
	}

}

void ACMainPayer::OnHorizontalLook(float InAxis)
{
	float rate = 45.0f;
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACMainPayer::OnVerticalLook(float InAxis)
{
	float rate = 45.0f;
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACMainPayer::Jump()
{
	if(Status->CanMove())
		ACharacter::Jump();
}

void ACMainPayer::JumpEnd()
{
	if (Status->CanMove())
		ACharacter::StopJumping();
}

void ACMainPayer::Run()
{
	if (Status->GetStamina() > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
		bRun = true;
	}
	
}

void ACMainPayer::RunEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	bRun = false;
	
}

void ACMainPayer::Equip()
{
	if (WeaponEquip != nullptr)
		WeaponEquip->DestroyWeapon();
	if (Weapon)
	{
		WeaponEquip = Weapon;
		Weapon->Equip(this);
		if (WeaponMode == EWeaponMode::Sword)
		{
			SkillsetsWidget->AddToViewport();
			SkillsetsWidget->SetVisibility(ESlateVisibility::Visible);
			AxeSkillsetsWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (WeaponMode == EWeaponMode::Axe)
		{
			AxeSkillsetsWidget->AddToViewport();
			SkillsetsWidget->SetVisibility(ESlateVisibility::Hidden);
			AxeSkillsetsWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ACMainPayer::Attack()
{
	if (WeaponMode == EWeaponMode::Sword)
	{
		if (NextCombo > 3)
			NextCombo = 0;
	}
	else if (WeaponMode == EWeaponMode::Axe)
	{
		if (NextCombo > 2)
			NextCombo = 0;
	}

	if (bCombo)
		bNextCombo = true;

	if (WeaponMode==EWeaponMode::Sword&&!State->IsAttackMode())
	{
		State->SetAttackMode();
		Status->SetStop();
		
		AttackMontage();
	}
	
	if (WeaponMode == EWeaponMode::Axe && !State->IsAttackMode())
	{
		State->SetAttackMode();
		Status->SetStop();

		AttackMontage();
	}
}

void ACMainPayer::QSkillAttack()
{
	if (WeaponMode == EWeaponMode::Sword && Status->CanMove())
	{
		State->SetAttackMode();
		Status->SetStop();
		WeaponEquip->SetPower(30.0f);
		PlayAnimMontage(QSkill[0]);
	}
	else if(WeaponMode == EWeaponMode::Axe && Status->CanMove())
	{
		State->SetAttackMode();
		Status->SetStop();
		WeaponEquip->SetPower(45.0f);
		PlayAnimMontage(QSkill[1]);
	}
}

void ACMainPayer::ESkillAttack()
{
	if (WeaponMode == EWeaponMode::Sword && Status->CanMove())
	{
		State->SetAttackMode();
		Status->SetStop();
		WeaponEquip->SetPower(25.0f);
		PlayAnimMontage(ESkill[0]);
	}
	else if (WeaponMode == EWeaponMode::Axe && Status->CanMove())
	{
		State->SetAttackMode();
		Status->SetStop();
		WeaponEquip->SetPower(5.0f);
		PlayAnimMontage(ESkill[1]);
	}
}

void ACMainPayer::Dead()
{
	if (State->IsDeadMode())
		PlayAnimMontage(MontageDead);
}

void ACMainPayer::DeadAffter()
{
	Destroy();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void ACMainPayer::Heal()
{
	Status->AddHealth(20.0f);
	HelthBarWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
}

void ACMainPayer::SubSpider()
{
	if (SpiderCount > 0)
		SpiderCount--;
	QuestWidget->SpiderUpdate(SpiderCount);
}

void ACMainPayer::SubMinion()
{
	if(MinionCount>0)
		MinionCount--;
	QuestWidget->MinionUpdate(MinionCount);
}

void ACMainPayer::AttackMontage()
{
	if(!State->IsDeadMode()&&WeaponMode == EWeaponMode::Sword)
		PlayAnimMontage(MontageSword[NextCombo]);
	else if (!State->IsDeadMode() && WeaponMode == EWeaponMode::Axe)
		PlayAnimMontage(MontageAxe[NextCombo]);
}

void ACMainPayer::Save()
{
	SaveGameInstance = NewObject<UCSaveGame>();
	if (SaveGameInstance)
	{
		SaveGameInstance->bStartMenu = false;
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0) == false)
			return;
	}
}

void ACMainPayer::Load()
{
	LoadGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (LoadGameInstance == nullptr)
	{
		return;
	}
	bShow = LoadGameInstance->bStartMenu;
}

void ACMainPayer::Nextcombo()
{
	if (bNextCombo)
	{
		NextCombo++;
		AttackMontage();
	}
	bNextCombo = false;
	
}

void ACMainPayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass()))
	{
		if (OtherActor->IsA(ACSpawnWeapon::StaticClass()))
		{
			Weapon = Cast<ACSpawnWeapon>(OtherActor);
		}
		else if (OtherActor->IsA(ACAxeWeapon::StaticClass()))
		{
			Weapon = Cast<ACAxeWeapon>(OtherActor);
		}
	}
}

void ACMainPayer::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Weapon = nullptr;
}

void ACMainPayer::UnarmedMode()
{
	WeaponMode = EWeaponMode::Unarmed;
}

void ACMainPayer::SwordMode()
{
	WeaponMode = EWeaponMode::Sword;
}

void ACMainPayer::AxeMode()
{
	WeaponMode = EWeaponMode::Axe;
}

void ACMainPayer::End_bAttackMode()
{
	State->SetIdleMode();
	Status->SetMove();
	if (WeaponMode == EWeaponMode::Sword)
		WeaponEquip->SetPower(10.0f);
	NextCombo = 0;
}

void ACMainPayer::CollisionOn()
{
	WeaponEquip->ActivityCollision();
}

void ACMainPayer::CollisionOff()
{
	WeaponEquip->DeactivateCollision();
}

void ACMainPayer::Hitted(float damage)
{
	State->SetHittedMode();

	Status->SubHealth(damage);

	HelthBarWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());

	if (HitSound&&!State->IsDeadMode())
	{
		UGameplayStatics::PlaySound2D(this, HitSound);
	}

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		Dead();
	}
}

void ACMainPayer::HittedEnd()
{
	State->SetIdleMode();
}

