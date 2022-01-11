#include "CAnimInstance_EnemyMinion.h"
#include "Global.h"
#include "Enemy/CEnemyMinion.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance_EnemyMinion::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);
}

void UCAnimInstance_EnemyMinion::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());

	EnemyMinion = Cast<ACEnemyMinion>(character);
	Falling = EnemyMinion->GetCharacterMovement()->IsFalling();
}