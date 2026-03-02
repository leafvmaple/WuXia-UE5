// WuxiaDialogueWidget.h — 对话 UI Widget
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaDialogueWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceSelected, int32, TargetNodeID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS()
class WUXIACORE_API UWuxiaDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ---- 显示对话节点 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowDialogueNode(const FWuxiaDialogueNode& Node);

	// ---- 显示选项列表 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowChoices(const TArray<FWuxiaDialogueChoice>& Choices);

	// ---- 清空选项 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ClearChoices();

	// ---- 逐字显示控制 ----
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void StartTypewriterEffect(const FText& FullText, float CharactersPerSecond);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void SkipTypewriter();

	// ---- 事件 ----
	UPROPERTY(BlueprintAssignable) FOnDialogueChoiceSelected OnChoiceSelected;
	UPROPERTY(BlueprintAssignable) FOnDialogueFinished OnDialogueFinished;
};
