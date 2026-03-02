// WuxiaBattleHUDWidget.cpp
#include "UI/Combat/WuxiaBattleHUDWidget.h"
#include "Combat/WuxiaBattleManager.h"

void UWuxiaBattleHUDWidget::BindBattleManager(AWuxiaBattleManager* InManager)
{
	BattleManager = InManager;
}
