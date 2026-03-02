// WuxiaDialogueComponent.h — 对话控制组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaDialogueComponent.generated.h"

class UWuxiaDialogueData;
class UWuxiaDialogueWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS(ClassGroup = (Wuxia), meta = (BlueprintSpawnableComponent))
class WUXIACORE_API UWuxiaDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWuxiaDialogueComponent();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Dialogue")
	void StartDialogue(UWuxiaDialogueData* InData);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Dialogue")
	void AdvanceDialogue();

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Dialogue")
	void SelectChoice(int32 TargetNodeID);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Dialogue")
	void EndDialogue();

	UFUNCTION(BlueprintPure, Category = "Wuxia|Dialogue")
	bool IsInDialogue() const { return bInDialogue; }

	// ---- 事件 ----
	UPROPERTY(BlueprintAssignable) FOnDialogueStarted OnDialogueStarted;
	UPROPERTY(BlueprintAssignable) FOnDialogueEnded OnDialogueEnded;

	// ---- UI ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wuxia|Dialogue")
	TSubclassOf<UWuxiaDialogueWidget> DialogueWidgetClass;

protected:
	UPROPERTY() TObjectPtr<UWuxiaDialogueData> ActiveDialogueData;
	UPROPERTY() TObjectPtr<UWuxiaDialogueWidget> DialogueWidget;

	int32 CurrentNodeID = 0;
	bool bInDialogue = false;

	void ShowCurrentNode();
};
