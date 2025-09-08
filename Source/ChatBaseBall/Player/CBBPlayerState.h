#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CBBPlayerState.generated.h"

UCLASS()
class CHATBASEBALL_API ACBBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACBBPlayerState();

	FString GetPlayerInfoString();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

};
