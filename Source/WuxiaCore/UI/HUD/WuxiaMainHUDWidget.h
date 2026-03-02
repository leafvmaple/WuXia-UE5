// WuxiaMainHUDWidget.h — 探索模式主 HUD
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WuxiaMainHUDWidget.generated.h"

UCLASS()
class WUXIACORE_API UWuxiaMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 显示/隐藏交互提示（如 "按E交互"） */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowInteractionPrompt(const FText& PromptText);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void HideInteractionPrompt();

	/** 更新队伍头像栏 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void UpdatePartyPortraits();

	/** 显示简短通知 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Wuxia|UI")
	void ShowNotification(const FText& Message, float Duration);
};
