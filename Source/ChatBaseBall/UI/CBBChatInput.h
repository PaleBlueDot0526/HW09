#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CBBChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class CHATBASEBALL_API UCBBChatInput : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_CI;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
};
