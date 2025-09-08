#include "Game/CBBGameModeBase.h"

#include "Game/CBBGameStateBase.h"
#include "Player/CBBPlayerController.h"
#include "Player/CBBPlayerState.h"

#include "EngineUtils.h"

void ACBBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ACBBPlayerController* CBBPC = Cast<ACBBPlayerController>(NewPlayer);
	if (IsValid(CBBPC))
	{
		CBBPC->NotificationText = FText::FromString(TEXT("Connected To The Game Server."));

		AllPlayerControllers.Add(CBBPC);

		ACBBPlayerState* CBBPS = CBBPC->GetPlayerState<ACBBPlayerState>();
		if (IsValid(CBBPS))
		{
			CBBPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACBBGameStateBase* CBBGS = GetGameState<ACBBGameStateBase>();
		if (IsValid(CBBGS))
		{
			CBBGS->MulticastRPCBroadcastLoginMessage(CBBPS->PlayerNameString);
		}

	}
}

void ACBBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Warning, TEXT("Answer : %s"), *SecretNumberString);
}

void ACBBGameModeBase::PrintChatMessageString(ACBBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<ACBBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBBPlayerController* CBBPC = *It;
			if (IsValid(CBBPC))
			{
				FString CombineMessageString = InChatMessageString + TEXT(" => ") + JudgeResultString;
				
				CBBPC->ClientRPCPrintChatMessageString(CombineMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else 
	{
		for (TActorIterator<ACBBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBBPlayerController* CBBPC = *It;
			if (IsValid(CBBPC))
			{
				CBBPC->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

FString ACBBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;

	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;

	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACBBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
			break;

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;

		for (TCHAR Char : InNumberString)
		{
			if (FChar::IsDigit(Char) == false || Char == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(Char);
		}

		if (bIsUnique == false)
			break;

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACBBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0;
	int32 BallConut = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			++StrikeCount;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				++BallConut;
			}
		}
	}

	if (StrikeCount == 0 && BallConut == 0)
	{
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallConut);
}

void ACBBGameModeBase::IncreaseGuessCount(ACBBPlayerController* InChattingPlayerController)
{
	ACBBPlayerState* CBBPS = InChattingPlayerController->GetPlayerState<ACBBPlayerState>();

	if (IsValid(CBBPS))
	{
		CBBPS->CurrentGuessCount++;
	}
}

void ACBBGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CBBPlayerController : AllPlayerControllers)
	{
		ACBBPlayerState* CBBPS = CBBPlayerController->GetPlayerState<ACBBPlayerState>();
		if (IsValid(CBBPS))
		{
			CBBPS->CurrentGuessCount = 0;
		}
	}
}

void ACBBGameModeBase::JudgeGame(ACBBPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ACBBPlayerState* CBBPS = InChattingPlayerController->GetPlayerState<ACBBPlayerState>();
		
		for (const auto& CBBPlayerController : AllPlayerControllers)
		{
			if (IsValid(CBBPS))
			{
				FString CombineMessageString = CBBPS->PlayerNameString + TEXT("Has Won This Game.");
				CBBPlayerController->NotificationText = FText::FromString(CombineMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		
		for (const auto& CBBPlayerController : AllPlayerControllers)
		{
			ACBBPlayerState* CBBPS = CBBPlayerController->GetPlayerState<ACBBPlayerState>();
			
			if (IsValid(CBBPS))
			{
				if (CBBPS->CurrentGuessCount < CBBPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (bIsDraw == true)	
		{
			for (const auto& CBBPlayerController : AllPlayerControllers)
			{
				CBBPlayerController->NotificationText = FText::FromString(TEXT("This Game is Draw..."));

				ResetGame();
			}
		}
	}
}
