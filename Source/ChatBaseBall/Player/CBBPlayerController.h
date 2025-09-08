#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBBPlayerController.generated.h"

class UCBBChatInput;
class UUSerWidget;

UCLASS()
class CHATBASEBALL_API ACBBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACBBPlayerController();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Chat")
	TSubclassOf<UCBBChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCBBChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	FString ChatMessageString;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

};
