#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEnemy.generated.h"


UCLASS()
class CPP_PROJECT_API ACEnemy : public ACharacter
{
	GENERATED_BODY()
		
public:
	ACEnemy();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMinTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMaxTime = 3.5f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* HitParticle;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void AttackEnd();
	void HittedEnd();

	void CollisionOn();
	void CollisionOff();

	void DeadAffter();
private:
	UFUNCTION()
		void OnBeginOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapCombat(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnBeginOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Attack();
	void Hit();
	void Dead();
	
	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
	
	UPROPERTY(EditAnywhere)
		class USoundCue* HitSound;
protected:
	//Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	UPROPERTY(EditDefaultsOnly)
		class UCStatusComponent* Status;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCEnemyHelath> SelectWidgetClass;

	class UCEnemyHelath* HelthBarWidget;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CombatCollision;
	
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* AttackCollision;

private:
	class UAnimMontage* MontageAttack;
	class UAnimMontage* MontageHit;
	class UAnimMontage* MontageDead;
	class ACMainPayer* Player;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
private:
	bool bCombatCollisionSphere = false;

	class AController* DamageInstigator;

	float Power = 15.0f;

	TArray<ACMainPayer*> playerHittedCheck;
};
