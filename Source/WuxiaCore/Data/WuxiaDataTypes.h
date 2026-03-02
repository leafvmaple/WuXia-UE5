// WuxiaDataTypes.h — 全局共用枚举与结构体
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WuxiaDataTypes.generated.h"

// ========== 游戏模式状态 ==========
UENUM(BlueprintType)
enum class EWuxiaGamePhase : uint8
{
	Exploration		UMETA(DisplayName = "探索"),
	Dialogue		UMETA(DisplayName = "对话"),
	Battle			UMETA(DisplayName = "战斗"),
	Menu			UMETA(DisplayName = "菜单")
};

// ========== 角色阵营 ==========
UENUM(BlueprintType)
enum class EWuxiaFaction : uint8
{
	Player			UMETA(DisplayName = "我方"),
	Enemy			UMETA(DisplayName = "敌方"),
	Neutral			UMETA(DisplayName = "中立")
};

// ========== 伤害类型 ==========
UENUM(BlueprintType)
enum class EWuxiaDamageType : uint8
{
	External		UMETA(DisplayName = "外功"),
	Internal		UMETA(DisplayName = "内功"),
	Poison			UMETA(DisplayName = "毒功")
};

// ========== 物品类型 ==========
UENUM(BlueprintType)
enum class EWuxiaItemType : uint8
{
	Weapon			UMETA(DisplayName = "武器"),
	Armor			UMETA(DisplayName = "防具"),
	Consumable		UMETA(DisplayName = "消耗品"),
	SecretManual	UMETA(DisplayName = "秘籍"),
	KeyItem			UMETA(DisplayName = "关键物品")
};

// ========== 装备槽位 ==========
UENUM(BlueprintType)
enum class EWuxiaEquipSlot : uint8
{
	Weapon			UMETA(DisplayName = "武器"),
	Head			UMETA(DisplayName = "头部"),
	Body			UMETA(DisplayName = "身体"),
	Feet			UMETA(DisplayName = "脚部"),
	Accessory		UMETA(DisplayName = "饰品")
};

// ========== 地形类型 ==========
UENUM(BlueprintType)
enum class EBattleTerrainType : uint8
{
	Plain			UMETA(DisplayName = "平地"),
	Obstacle		UMETA(DisplayName = "障碍"),
	Water			UMETA(DisplayName = "水域"),
	HighGround		UMETA(DisplayName = "高地")
};

// ========== 战斗行动类型 ==========
UENUM(BlueprintType)
enum class EBattleActionType : uint8
{
	Move			UMETA(DisplayName = "移动"),
	Attack			UMETA(DisplayName = "攻击"),
	Ability			UMETA(DisplayName = "武功"),
	Item			UMETA(DisplayName = "物品"),
	Defend			UMETA(DisplayName = "防御"),
	Flee			UMETA(DisplayName = "逃跑")
};

// ========== 战斗状态 ==========
UENUM(BlueprintType)
enum class EBattleState : uint8
{
	None,
	Init,
	TurnOrder,
	PlayerTurn,
	EnemyTurn,
	CheckResult,
	Victory,
	Defeat
};

// ========== 精灵方向 ==========
UENUM(BlueprintType)
enum class EWuxiaSpriteDirection : uint8
{
	Down	= 0		UMETA(DisplayName = "下"),
	Left	= 1		UMETA(DisplayName = "左"),
	Right	= 2		UMETA(DisplayName = "右"),
	Up		= 3		UMETA(DisplayName = "上")
};

// ========== 精灵动画状态 ==========
UENUM(BlueprintType)
enum class EWuxiaSpriteAnimState : uint8
{
	Idle,
	Walk,
	Attack,
	Hit,
	Dead
};

// ========== 攻击范围模板 ==========
UENUM(BlueprintType)
enum class EAbilityRangePattern : uint8
{
	Cross			UMETA(DisplayName = "十字"),
	Diamond			UMETA(DisplayName = "菱形"),
	Line			UMETA(DisplayName = "直线"),
	Square			UMETA(DisplayName = "方形"),
	FullScreen		UMETA(DisplayName = "全屏"),
	Single			UMETA(DisplayName = "单体")
};

// ========== 战斗格子数据 ==========
USTRUCT(BlueprintType)
struct FBattleGridCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Coordinate = FIntPoint::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBattleTerrainType TerrainType = EBattleTerrainType::Plain;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> OccupyingActor = nullptr;

	bool IsPassable() const { return TerrainType != EBattleTerrainType::Obstacle && !OccupyingActor.IsValid(); }
};

// ========== 对话节点 ==========
USTRUCT(BlueprintType)
struct FWuxiaDialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NodeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText DialogueText;

	/** 下一个节点 ID，-1 表示结束 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NextNodeID = -1;
};

// ========== 对话选项 ==========
USTRUCT(BlueprintType)
struct FWuxiaDialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 OwnerNodeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetNodeID = 0;

	/** 条件标签，为空则无条件 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ConditionTag;
};
