#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController_Minion.generated.h"

UCLASS()
class CPP_PROJECT_API ACAIController_Minion : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;
public:
	ACAIController_Minion();

	float GetSightRadius();
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
private:
	class ACEnemyMinion* OwnerEnemy;

	class UAISenseConfig_Sight* Sight;
};
