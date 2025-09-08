#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CBBGameStateBase.generated.h"

UCLASS()
class CHATBASEBALL_API ACBBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
};
