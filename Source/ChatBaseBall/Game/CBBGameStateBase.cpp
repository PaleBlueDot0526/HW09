#include "Game/CBBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CBBPlayerController.h"

void ACBBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			ACBBPlayerController* CBBPC = Cast<ACBBPlayerController>(PC);
			if (IsValid(CBBPC))
			{
				FString NotificationString = InNameString + TEXT(" has Join the Game.");
				CBBPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
