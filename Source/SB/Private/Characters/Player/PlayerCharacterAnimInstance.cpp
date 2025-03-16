#include "Characters/Player/PlayerCharacterAnimInstance.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());	
	if (PlayerCharacter)
	{
		PlayerMovementComponent = PlayerCharacter->GetCharacterMovement();
	}

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PlayerCharacter)
	{
		Velocity = PlayerCharacter->GetVelocity();
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		IsFalling = PlayerCharacter->GetCharacterMovement()->IsFalling();
		ShouldMove = GroundSpeed > 3.0f && PlayerMovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	}
}
