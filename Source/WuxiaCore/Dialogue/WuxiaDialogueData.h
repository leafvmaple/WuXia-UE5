// WuxiaDialogueData.h — 对话数据资产
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaDialogueData.generated.h"

UCLASS(BlueprintType)
class WUXIACORE_API UWuxiaDialogueData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "对话")
	TArray<FWuxiaDialogueNode> Nodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "对话")
	TArray<FWuxiaDialogueChoice> Choices;

	/** 获取起始节点（NodeID == 0） */
	UFUNCTION(BlueprintPure, Category = "Wuxia|Dialogue")
	FWuxiaDialogueNode GetStartNode() const;

	/** 获取指定 ID 的节点 */
	UFUNCTION(BlueprintPure, Category = "Wuxia|Dialogue")
	bool GetNodeByID(int32 NodeID, FWuxiaDialogueNode& OutNode) const;

	/** 获取某节点的所有选项 */
	UFUNCTION(BlueprintPure, Category = "Wuxia|Dialogue")
	TArray<FWuxiaDialogueChoice> GetChoicesForNode(int32 NodeID) const;
};
