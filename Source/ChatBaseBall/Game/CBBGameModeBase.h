#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CBBGameModeBase.generated.h"

class ACBBPlayerController;

UCLASS()
class CHATBASEBALL_API ACBBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	void PrintChatMessageString(ACBBPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void IncreaseGuessCount(ACBBPlayerController* InChattingPlayerController);

	void ResetGame();
	void JudgeGame(ACBBPlayerController* InChattingPlayerController, int InStrikeCount);


protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACBBPlayerController>> AllPlayerControllers;
};
