#include "Player/CBBPlayerState.h"

#include "Net/UnrealNetwork.h"

ACBBPlayerState::ACBBPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount()
	, MaxGuessCount(3)
{
	bReplicates = true;
}

FString ACBBPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount)
		+ TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");

	return PlayerInfoString;
}

void ACBBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
//	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

