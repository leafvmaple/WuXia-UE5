// WuxiaBattleManager.cpp
#include "Combat/WuxiaBattleManager.h"
#include "Combat/WuxiaBattleGrid.h"
#include "Character/WuxiaCharacterBase.h"
#include "Character/WuxiaAttributeSet.h"

AWuxiaBattleManager::AWuxiaBattleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWuxiaBattleManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWuxiaBattleManager::InitBattle()
{
	// 生成战斗网格
	if (BattleGridClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		BattleGrid = GetWorld()->SpawnActor<AWuxiaBattleGrid>(BattleGridClass, GetActorLocation(), FRotator::ZeroRotator, Params);
	}
	else
	{
		BattleGrid = GetWorld()->SpawnActor<AWuxiaBattleGrid>(AWuxiaBattleGrid::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
	}

	if (BattleGrid)
	{
		BattleGrid->InitGrid(GridWidth, GridHeight);
	}

	SetBattleState(EBattleState::Init);
	GenerateTurnOrder();
	SetBattleState(EBattleState::TurnOrder);
	StartNextTurn();
}

void AWuxiaBattleManager::AddCombatant(AWuxiaCharacterBase* Character, FIntPoint SpawnCell)
{
	if (!Character || !BattleGrid) return;

	Combatants.Add(Character);
	Character->GridPosition = SpawnCell;
	Character->SetActorLocation(BattleGrid->GridToWorld(SpawnCell));
	BattleGrid->SetCellOccupant(SpawnCell, Character);
}

void AWuxiaBattleManager::GenerateTurnOrder()
{
	TurnOrder = Combatants;

	// 按轻功(Agility)降序排列
	TurnOrder.Sort([](const TObjectPtr<AWuxiaCharacterBase>& A, const TObjectPtr<AWuxiaCharacterBase>& B)
	{
		float AgilityA = A->GetAttributeSet() ? A->GetAttributeSet()->GetAgility() : 0.f;
		float AgilityB = B->GetAttributeSet() ? B->GetAttributeSet()->GetAgility() : 0.f;
		return AgilityA > AgilityB;
	});

	CurrentTurnIndex = 0;
}

void AWuxiaBattleManager::StartNextTurn()
{
	// 跳过已死亡的角色
	while (CurrentTurnIndex < TurnOrder.Num())
	{
		AWuxiaCharacterBase* Current = TurnOrder[CurrentTurnIndex].Get();
		if (Current && Current->IsAlive())
		{
			break;
		}
		CurrentTurnIndex++;
	}

	// 如果所有人都行动完毕，重新生成回合
	if (CurrentTurnIndex >= TurnOrder.Num())
	{
		// 先检查战斗结果
		EBattleState Result = CheckBattleResult();
		if (Result == EBattleState::Victory || Result == EBattleState::Defeat)
		{
			SetBattleState(Result);
			return;
		}

		GenerateTurnOrder();
	}

	AWuxiaCharacterBase* ActiveChar = GetActiveCharacter();
	if (!ActiveChar) return;

	if (ActiveChar->Faction == EWuxiaFaction::Player)
	{
		SetBattleState(EBattleState::PlayerTurn);
	}
	else
	{
		SetBattleState(EBattleState::EnemyTurn);
		ProcessAITurn();
	}

	OnTurnStarted.Broadcast(ActiveChar);
}

AWuxiaCharacterBase* AWuxiaBattleManager::GetActiveCharacter() const
{
	if (CurrentTurnIndex < TurnOrder.Num())
	{
		return TurnOrder[CurrentTurnIndex].Get();
	}
	return nullptr;
}

void AWuxiaBattleManager::ExecuteAction(EBattleActionType Action, FIntPoint TargetCell)
{
	AWuxiaCharacterBase* ActiveChar = GetActiveCharacter();
	if (!ActiveChar || !BattleGrid) return;

	switch (Action)
	{
	case EBattleActionType::Move:
	{
		BattleGrid->SetCellOccupant(ActiveChar->GridPosition, nullptr);
		ActiveChar->GridPosition = TargetCell;
		ActiveChar->SetActorLocation(BattleGrid->GridToWorld(TargetCell));
		BattleGrid->SetCellOccupant(TargetCell, ActiveChar);
		break;
	}

	case EBattleActionType::Attack:
	{
		// 基础普通攻击
		const FBattleGridCell& Cell = BattleGrid->GetCell(TargetCell);
		AWuxiaCharacterBase* Target = Cast<AWuxiaCharacterBase>(Cell.OccupyingActor.Get());
		if (Target && Target->GetAttributeSet() && ActiveChar->GetAttributeSet())
		{
			float Damage = FMath::Max(0.f, ActiveChar->GetAttributeSet()->GetAttack() - Target->GetAttributeSet()->GetDefense());
			// 通过 GAS 处理更好，这里简化直接扣血
			float NewHP = Target->GetAttributeSet()->GetHealth() - Damage;
			Target->GetAttributeSet()->SetHealth(FMath::Max(0.f, NewHP));
		}
		break;
	}

	case EBattleActionType::Defend:
		// 防御：本回合防御力翻倍（通过 GameplayEffect 实现更佳）
		break;

	case EBattleActionType::Flee:
		// 逃跑逻辑
		break;

	default:
		break;
	}
}

void AWuxiaBattleManager::EndCurrentTurn()
{
	BattleGrid->ClearHighlight();

	EBattleState Result = CheckBattleResult();
	if (Result == EBattleState::Victory || Result == EBattleState::Defeat)
	{
		SetBattleState(Result);
		return;
	}

	CurrentTurnIndex++;
	StartNextTurn();
}

EBattleState AWuxiaBattleManager::CheckBattleResult() const
{
	bool bAnyPlayerAlive = false;
	bool bAnyEnemyAlive = false;

	for (const auto& Combatant : Combatants)
	{
		if (!Combatant || !Combatant->IsAlive()) continue;

		if (Combatant->Faction == EWuxiaFaction::Player)
			bAnyPlayerAlive = true;
		else if (Combatant->Faction == EWuxiaFaction::Enemy)
			bAnyEnemyAlive = true;
	}

	if (!bAnyEnemyAlive) return EBattleState::Victory;
	if (!bAnyPlayerAlive) return EBattleState::Defeat;
	return EBattleState::None; // 战斗继续
}

void AWuxiaBattleManager::ProcessAITurn()
{
	AWuxiaCharacterBase* AIChar = GetActiveCharacter();
	if (!AIChar || !BattleGrid) return;

	// 简单 AI：找最近的敌方角色，移动靠近并攻击
	AWuxiaCharacterBase* ClosestTarget = nullptr;
	int32 MinDist = INT_MAX;

	for (const auto& Combatant : Combatants)
	{
		if (!Combatant || !Combatant->IsAlive()) continue;
		if (Combatant->Faction == AIChar->Faction) continue;

		int32 Dist = FMath::Abs(Combatant->GridPosition.X - AIChar->GridPosition.X)
			+ FMath::Abs(Combatant->GridPosition.Y - AIChar->GridPosition.Y);
		if (Dist < MinDist)
		{
			MinDist = Dist;
			ClosestTarget = Combatant.Get();
		}
	}

	if (ClosestTarget)
	{
		// 尝试移动靠近
		UWuxiaAttributeSet* Attrs = AIChar->GetAttributeSet();
		int32 MoveRange = Attrs ? FMath::FloorToInt(Attrs->GetMoveRange()) : 3;
		TArray<FIntPoint> Reachable = BattleGrid->GetReachableCells(AIChar->GridPosition, MoveRange);

		FIntPoint BestCell = AIChar->GridPosition;
		int32 BestDist = MinDist;

		for (const FIntPoint& Cell : Reachable)
		{
			int32 D = FMath::Abs(Cell.X - ClosestTarget->GridPosition.X)
				+ FMath::Abs(Cell.Y - ClosestTarget->GridPosition.Y);
			if (D < BestDist)
			{
				BestDist = D;
				BestCell = Cell;
			}
		}

		if (BestCell != AIChar->GridPosition)
		{
			ExecuteAction(EBattleActionType::Move, BestCell);
		}

		// 如果相邻则攻击
		if (BestDist <= 1)
		{
			ExecuteAction(EBattleActionType::Attack, ClosestTarget->GridPosition);
		}
	}

	// AI 回合结束
	EndCurrentTurn();
}

void AWuxiaBattleManager::SetBattleState(EBattleState NewState)
{
	CurrentState = NewState;
	OnBattleStateChanged.Broadcast(NewState);
}
