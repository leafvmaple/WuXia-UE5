// WuxiaDialogueData.cpp
#include "Dialogue/WuxiaDialogueData.h"

FWuxiaDialogueNode UWuxiaDialogueData::GetStartNode() const
{
	for (const auto& Node : Nodes)
	{
		if (Node.NodeID == 0) return Node;
	}
	return FWuxiaDialogueNode();
}

bool UWuxiaDialogueData::GetNodeByID(int32 NodeID, FWuxiaDialogueNode& OutNode) const
{
	for (const auto& Node : Nodes)
	{
		if (Node.NodeID == NodeID)
		{
			OutNode = Node;
			return true;
		}
	}
	return false;
}

TArray<FWuxiaDialogueChoice> UWuxiaDialogueData::GetChoicesForNode(int32 NodeID) const
{
	TArray<FWuxiaDialogueChoice> Result;
	for (const auto& Choice : Choices)
	{
		if (Choice.OwnerNodeID == NodeID)
		{
			Result.Add(Choice);
		}
	}
	return Result;
}
