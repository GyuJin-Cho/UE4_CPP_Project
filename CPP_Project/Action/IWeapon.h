#pragma once

#include "Player/CMainPayer.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

UINTERFACE(MinimalAPI)
class UIWeapon : public UInterface
{
	GENERATED_BODY()
};

class CPP_PROJECT_API IIWeapon
{
	GENERATED_BODY()

protected:
	TArray<class ACharacter*> HittedCharacter;
public:
	virtual void Equip(ACMainPayer* player) = 0;
	virtual void ActivityCollision() = 0;
	virtual void DeactivateCollision() = 0;
	virtual void SetPower(float power) = 0;
	virtual void DestroyWeapon() = 0;
};
