#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWeapon.h"
#include "CSpawnWeapon.generated.h"

UCLASS()
class CPP_PROJECT_API ACSpawnWeapon : public AActor, public IIWeapon
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
		class USoundCue* EuipSound;
public:	
	ACSpawnWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void Equip(ACMainPayer* player) override;
	void ActivityCollision() override;
	void DeactivateCollision() override;
	void SetPower(float power) override;
	void DestroyWeapon() override;
public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(EditAnywhere)
		float YawRolling = 1.0f;
	float Yaw = 0.0f;
	
	float Power = 10.0f;

	bool Rotate = true;

	bool IsEquip = false;
};
