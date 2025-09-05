#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBBPlayerController.generated.h"

class UCBBChatInput;

UCLASS()
class CHATBASEBALL_API ACBBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Chat")
	TSubclassOf<UCBBChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCBBChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
};
