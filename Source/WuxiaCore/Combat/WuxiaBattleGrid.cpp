// WuxiaBattleGrid.cpp
#include "Combat/WuxiaBattleGrid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AWuxiaBattleGrid::AWuxiaBattleGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridVisualization = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridVis"));
	GridVisualization->SetupAttachment(RootComponent);
	GridVisualization->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HighlightVisualization = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("HighlightVis"));
	HighlightVisualization->SetupAttachment(RootComponent);
	HighlightVisualization->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWuxiaBattleGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AWuxiaBattleGrid::InitGrid(int32 InWidth, int32 InHeight)
{
	GridWidth = InWidth;
	GridHeight = InHeight;
	Cells.Empty();
	Cells.SetNum(GridWidth * GridHeight);

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			FBattleGridCell& Cell = Cells[Y * GridWidth + X];
			Cell.Coordinate = FIntPoint(X, Y);
			Cell.TerrainType = EBattleTerrainType::Plain;
			Cell.OccupyingActor = nullptr;
		}
	}
}

bool AWuxiaBattleGrid::IsValidCell(FIntPoint Coord) const
{
	return Coord.X >= 0 && Coord.X < GridWidth && Coord.Y >= 0 && Coord.Y < GridHeight;
}

FBattleGridCell AWuxiaBattleGrid::GetCell(FIntPoint Coord) const
{
	if (IsValidCell(Coord))
	{
		return Cells[CellIndex(Coord)];
	}
	return FBattleGridCell();
}

void AWuxiaBattleGrid::SetCellTerrain(FIntPoint Coord, EBattleTerrainType Terrain)
{
	if (IsValidCell(Coord))
	{
		Cells[CellIndex(Coord)].TerrainType = Terrain;
	}
}

void AWuxiaBattleGrid::SetCellOccupant(FIntPoint Coord, AActor* Occupant)
{
	if (IsValidCell(Coord))
	{
		Cells[CellIndex(Coord)].OccupyingActor = Occupant;
	}
}

FVector AWuxiaBattleGrid::GridToWorld(FIntPoint Coord) const
{
	return GetActorLocation() + FVector(Coord.X * CellSize + CellSize * 0.5f, Coord.Y * CellSize + CellSize * 0.5f, 0.f);
}

FIntPoint AWuxiaBattleGrid::WorldToGrid(FVector WorldPos) const
{
	const FVector Local = WorldPos - GetActorLocation();
	return FIntPoint(FMath::FloorToInt(Local.X / CellSize), FMath::FloorToInt(Local.Y / CellSize));
}

TArray<FIntPoint> AWuxiaBattleGrid::GetReachableCells(FIntPoint Start, int32 MoveRange) const
{
	TArray<FIntPoint> Result;
	if (!IsValidCell(Start)) return Result;

	// BFS
	TMap<int32, int32> CostMap; // CellIndex -> cost
	TArray<FIntPoint> Queue;
	Queue.Add(Start);
	CostMap.Add(CellIndex(Start), 0);

	static const FIntPoint Dirs[] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

	int32 Head = 0;
	while (Head < Queue.Num())
	{
		FIntPoint Current = Queue[Head++];
		int32 CurrentCost = CostMap[CellIndex(Current)];

		for (const FIntPoint& Dir : Dirs)
		{
			FIntPoint Next = Current + Dir;
			if (!IsValidCell(Next)) continue;

			const FBattleGridCell& NextCell = Cells[CellIndex(Next)];
			if (!NextCell.IsPassable()) continue;

			int32 NextCost = CurrentCost + 1;
			if (NextCost > MoveRange) continue;

			int32 NextIdx = CellIndex(Next);
			if (CostMap.Contains(NextIdx) && CostMap[NextIdx] <= NextCost) continue;

			CostMap.Add(NextIdx, NextCost);
			Queue.Add(Next);
			Result.Add(Next);
		}
	}

	return Result;
}

TArray<FIntPoint> AWuxiaBattleGrid::GetAbilityRangeCells(FIntPoint Center, EAbilityRangePattern Pattern, int32 Range) const
{
	TArray<FIntPoint> Result;

	switch (Pattern)
	{
	case EAbilityRangePattern::Single:
		if (IsValidCell(Center)) Result.Add(Center);
		break;

	case EAbilityRangePattern::Cross:
		for (int32 i = 1; i <= Range; ++i)
		{
			FIntPoint Up = Center + FIntPoint(0, -i);
			FIntPoint Down = Center + FIntPoint(0, i);
			FIntPoint Left = Center + FIntPoint(-i, 0);
			FIntPoint Right = Center + FIntPoint(i, 0);
			if (IsValidCell(Up)) Result.Add(Up);
			if (IsValidCell(Down)) Result.Add(Down);
			if (IsValidCell(Left)) Result.Add(Left);
			if (IsValidCell(Right)) Result.Add(Right);
		}
		break;

	case EAbilityRangePattern::Diamond:
		for (int32 DX = -Range; DX <= Range; ++DX)
		{
			for (int32 DY = -Range; DY <= Range; ++DY)
			{
				if (FMath::Abs(DX) + FMath::Abs(DY) <= Range && (DX != 0 || DY != 0))
				{
					FIntPoint P = Center + FIntPoint(DX, DY);
					if (IsValidCell(P)) Result.Add(P);
				}
			}
		}
		break;

	case EAbilityRangePattern::Square:
		for (int32 DX = -Range; DX <= Range; ++DX)
		{
			for (int32 DY = -Range; DY <= Range; ++DY)
			{
				if (DX != 0 || DY != 0)
				{
					FIntPoint P = Center + FIntPoint(DX, DY);
					if (IsValidCell(P)) Result.Add(P);
				}
			}
		}
		break;

	case EAbilityRangePattern::Line:
		// 四个方向各 Range 格
		for (int32 i = 1; i <= Range; ++i)
		{
			FIntPoint P = Center + FIntPoint(i, 0);
			if (IsValidCell(P)) Result.Add(P);
		}
		break;

	case EAbilityRangePattern::FullScreen:
		for (int32 X = 0; X < GridWidth; ++X)
		{
			for (int32 Y = 0; Y < GridHeight; ++Y)
			{
				if (X != Center.X || Y != Center.Y)
				{
					Result.Add(FIntPoint(X, Y));
				}
			}
		}
		break;
	}

	return Result;
}

void AWuxiaBattleGrid::ShowHighlight(const TArray<FIntPoint>& InCells, FLinearColor Color)
{
	ClearHighlight();

	for (const FIntPoint& Coord : InCells)
	{
		FVector WorldPos = GridToWorld(Coord);
		WorldPos.Z += 1.f; // 略微抬高
		FTransform T(FRotator::ZeroRotator, WorldPos, FVector(CellSize / 100.f));
		HighlightVisualization->AddInstance(T);
	}
}

void AWuxiaBattleGrid::ClearHighlight()
{
	HighlightVisualization->ClearInstances();
}
