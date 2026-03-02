// WuxiaBattleGrid.h — 战斗地图网格系统
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/WuxiaDataTypes.h"
#include "WuxiaBattleGrid.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class WUXIACORE_API AWuxiaBattleGrid : public AActor
{
	GENERATED_BODY()

public:
	AWuxiaBattleGrid();

	// ---- 网格初始化 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void InitGrid(int32 InWidth, int32 InHeight);

	// ---- 查询 ----
	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	bool IsValidCell(FIntPoint Coord) const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	FBattleGridCell GetCell(FIntPoint Coord) const;

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void SetCellTerrain(FIntPoint Coord, EBattleTerrainType Terrain);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void SetCellOccupant(FIntPoint Coord, AActor* Occupant);

	// ---- 坐标转换 ----
	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	FVector GridToWorld(FIntPoint Coord) const;

	UFUNCTION(BlueprintPure, Category = "Wuxia|Battle")
	FIntPoint WorldToGrid(FVector WorldPos) const;

	// ---- 可移动范围（BFS） ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	TArray<FIntPoint> GetReachableCells(FIntPoint Start, int32 MoveRange) const;

	// ---- 攻击范围 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	TArray<FIntPoint> GetAbilityRangeCells(FIntPoint Center, EAbilityRangePattern Pattern, int32 Range) const;

	// ---- 高亮显示 ----
	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void ShowHighlight(const TArray<FIntPoint>& Cells, FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "Wuxia|Battle")
	void ClearHighlight();

	// ---- 属性 ----
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|Battle")
	int32 GridWidth = 16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|Battle")
	int32 GridHeight = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wuxia|Battle")
	float CellSize = 100.f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY() TArray<FBattleGridCell> Cells;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> GridVisualization;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> HighlightVisualization;

	int32 CellIndex(FIntPoint Coord) const { return Coord.Y * GridWidth + Coord.X; }
};
