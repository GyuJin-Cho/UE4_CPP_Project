#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "CMainPayer.generated.h"

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
	Unarmed,
	Sword,
	Axe,
	MAX
};

UCLASS()
class CPP_PROJECT_API ACMainPayer : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
		uint8 TeamID = 0;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* HitParticle;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class USoundCue* HitSound;

public:
	ACMainPayer();

public:

	FORCEINLINE void EnableCombo() { bCombo = true; }
	FORCEINLINE void DisableCombo() { bCombo = false; }

	void UnarmedMode();
	void SwordMode();
	void AxeMode();
	void Nextcombo();
	void End_bAttackMode();
	void CollisionOn();
	void CollisionOff();

	void Hitted(float damage);
	void HittedEnd();

	void DeadAffter();
	void Heal();
	
	void SubSpider();
	void SubMinion();
private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	void Jump();
	void JumpEnd();
	void Run();
	void RunEnd();
	void Equip();
	void Attack();
	void QSkillAttack();
	void ESkillAttack();
	void Dead();

	void AttackMontage();
	void Save();
	void Load();
	

private:
	//Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
private:
	
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCHealthBar> SelectWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCStartMenu> StartLevelWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_SkillSets> SkillsetsClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_AxeSklii> AxeSkillsetsClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Quest> QuestClass;

	class UCStartMenu* StartLevelWidget;

	class UCHealthBar* HelthBarWidget;

	class UCUserWidget_SkillSets* SkillsetsWidget;
	class UCUserWidget_AxeSklii* AxeSkillsetsWidget;

	class UCUserWidget_Quest* QuestWidget;

	class IIWeapon* Weapon;
	class IIWeapon* WeaponEquip;
	class UAnimMontage* MontageSword[3];
	class UAnimMontage* MontageAxe[2];
	class UAnimMontage* MontageDead;
	class UAnimMontage* QSkill[2];
	class UAnimMontage* ESkill[2];
	class AController* DamageInstigator;
	class APlayerController* Controller;


private:
	EWeaponMode WeaponMode = EWeaponMode::Unarmed;
	bool bAttack = false;

	bool bCombo = false;
	bool bNextCombo = false;
	bool bShow = false;
	bool bRun = false;
	float StaminaDrainRate;
	FString SaveSlotName = "Save";
	int32 NextCombo = 0;
	int32 MinionCount = 8;
	int32 SpiderCount = 15;
	class UCSaveGame* SaveGameInstance;
	class UCSaveGame* LoadGameInstance;
};
