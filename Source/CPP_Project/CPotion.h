#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPotion.generated.h"

UCLASS()
class CPP_PROJECT_API ACPotion : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPotion();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		class USphereComponent* Sphere;

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

private:
	UPROPERTY(EditAnywhere)
		float YawRolling = 1.0f;
	float Yaw = 0.0f;
	bool Rotate = true;
};
