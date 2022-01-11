#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWeapon.h"
#include "CAxeWeapon.generated.h"

UCLASS()
class CPP_PROJECT_API ACAxeWeapon : public AActor, public IIWeapon
{
	GENERATED_BODY()
	
public:	
	ACAxeWeapon();

private:
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
		class USoundCue* EuipSound;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Particle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	void Equip(ACMainPayer* player) override;
	void ActivityCollision() override;
	void DeactivateCollision() override;
	void SetPower(float power) override;
	void DestroyWeapon() override;

private:
	UPROPERTY(EditAnywhere)
		float YawRolling = 1.0f;
	float Yaw = 0.0f;

	UPROPERTY(EditAnywhere)
		float Power = 25.0f;

	bool Rotate = true;
};
