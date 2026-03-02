// WuxiaDialogueComponent.cpp
#include "Dialogue/WuxiaDialogueComponent.h"
#include "Dialogue/WuxiaDialogueData.h"
#include "UI/Dialogue/WuxiaDialogueWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

UWuxiaDialogueComponent::UWuxiaDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWuxiaDialogueComponent::StartDialogue(UWuxiaDialogueData* InData)
{
	if (!InData) return;

	ActiveDialogueData = InData;
	CurrentNodeID = 0;
	bInDialogue = true;

	// 创建对话 Widget
	if (DialogueWidgetClass)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			DialogueWidget = CreateWidget<UWuxiaDialogueWidget>(PC, DialogueWidgetClass);
			if (DialogueWidget)
			{
				DialogueWidget->AddToViewport(10);
				DialogueWidget->OnChoiceSelected.AddDynamic(this, &UWuxiaDialogueComponent::SelectChoice);
			}
		}
	}

	OnDialogueStarted.Broadcast();
	ShowCurrentNode();
}

void UWuxiaDialogueComponent::ShowCurrentNode()
{
	if (!ActiveDialogueData || !DialogueWidget) return;

	FWuxiaDialogueNode Node;
	if (ActiveDialogueData->GetNodeByID(CurrentNodeID, Node))
	{
		DialogueWidget->ShowDialogueNode(Node);

		// 检查是否有选项
		TArray<FWuxiaDialogueChoice> Choices = ActiveDialogueData->GetChoicesForNode(CurrentNodeID);
		if (Choices.Num() > 0)
		{
			DialogueWidget->ShowChoices(Choices);
		}
		else
		{
			DialogueWidget->ClearChoices();
		}
	}
	else
	{
		EndDialogue();
	}
}

void UWuxiaDialogueComponent::AdvanceDialogue()
{
	if (!ActiveDialogueData) return;

	FWuxiaDialogueNode Node;
	if (ActiveDialogueData->GetNodeByID(CurrentNodeID, Node))
	{
		// 检查是否有选项（有选项时需要等待玩家选择）
		TArray<FWuxiaDialogueChoice> Choices = ActiveDialogueData->GetChoicesForNode(CurrentNodeID);
		if (Choices.Num() > 0)
		{
			return; // 等待玩家选择
		}

		if (Node.NextNodeID < 0)
		{
			EndDialogue();
		}
		else
		{
			CurrentNodeID = Node.NextNodeID;
			ShowCurrentNode();
		}
	}
}

void UWuxiaDialogueComponent::SelectChoice(int32 TargetNodeID)
{
	CurrentNodeID = TargetNodeID;
	ShowCurrentNode();
}

void UWuxiaDialogueComponent::EndDialogue()
{
	bInDialogue = false;
	ActiveDialogueData = nullptr;

	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
	}

	OnDialogueEnded.Broadcast();
}
