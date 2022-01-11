#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"
UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_PROJECT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxMarna = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = {200, 400, 600 };

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE float GetMaxMarna() { return MaxMarna; }
	FORCEINLINE float GetMarna() { return Marna; }

	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return Stamina; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

	FORCEINLINE bool CanMove() { return  bCanMove; }


public:	
	UCStatusComponent();

	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void AddMarna(float InAmount);
	void SubMarna(float InAmount);

	void AddStamina(float InAmount);
	void SubStamina(float InAmount);

protected:
	virtual void BeginPlay() override;

private:
	float Health = 100.0f;
	float Marna = 100.0f;
	float Stamina = 100.0f;

	bool bCanMove = true;
		
};
