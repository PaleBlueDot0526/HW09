#include "UI/CBBChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/CBBPlayerController.h"

void UCBBChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_CI->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_CI->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	
}

void UCBBChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_CI->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_CI->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCBBChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwingPC = GetOwningPlayer();
		if (IsValid(OwingPC))
		{
			ACBBPlayerController* OwingCBBPC = Cast<ACBBPlayerController>(OwingPC);
			if (IsValid(OwingCBBPC))
			{
				OwingCBBPC->SetChatMessageString(Text.ToString());

				EditableTextBox_CI->SetText(FText());
			}
		}
	}
}