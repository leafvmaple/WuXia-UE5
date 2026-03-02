# Plan: 武侠 HD2D 垂直切片

## 概述

在 UE 5.7 空项目的基础上，打造一个可玩的垂直切片：包含**一个城镇场景**（可与 NPC 对话交互）、**一段大地图探索**、以及**一场完整的战棋式回合制战斗**。视觉风格为 HD2D（3D 场景 + 2D 像素风角色 + 景深/光效），战斗采用原版金庸群侠传式网格移动 + 回合制攻击，支持最多 6 人队伍。使用 Placeholder 资源验证所有核心系统。

---

## 第一阶段：项目基础设施搭建

### Step 1 — 渲染管线重配（HD2D 适配）

修改 `Config/DefaultEngine.ini`：
- 开启前向渲染：`r.ForwardShading=True`（MSAA 生效前提）
- 保持 `r.MSAACount=4`（像素风精灵抗锯齿，不模糊纹理）
- 关闭 Lumen GI：`r.DynamicGlobalIlluminationMethod=0`，改用简化光照/烘焙
- 关闭 Lumen 反射：`r.ReflectionMethod=0`（Screen Space 或关闭）
- 关闭硬件光追：`r.Lumen.HardwareRayTracing=False`、`r.RayTracing=False`、`r.PathTracing=False`
- Nanite 可保留（3D 场景部分可用），但精灵不依赖
- 保持 Bloom 开启（HD2D 柔光效果核心），调低强度
- 保持自动曝光/动态模糊关闭（已正确）
- 后处理体积中配置 **Cinematic DOF**：焦距对准角色行走平面、光圈 f/1.4~2.8，实现 HD2D 标志性移轴景深效果

### Step 2 — 启用必要插件

修改 `WuxiaCore.uproject` 的 Plugins 数组，添加：
- `GameplayAbilities` — GAS 战斗能力系统
- `CommonUI` — UI 框架（菜单、对话框、多平台输入）
- `Paper2D` — 基础精灵支持（备用，主要用自定义 Billboard）
- `PCG` — 程序化场景装饰（可选）
- 保留 `EnhancedInput`（已配置）和 `ModelingToolsEditorMode`

### Step 3 — 模块依赖更新

修改 `Source/WuxiaCore/WuxiaCore.Build.cs`：
- `PublicDependencyModuleNames` 增加：`UMG`, `AIModule`, `NavigationSystem`
- `PrivateDependencyModuleNames` 增加：`Slate`, `SlateCore`, `GameplayAbilities`, `GameplayTags`, `GameplayTasks`, `CommonUI`, `CommonInput`, `Paper2D`

### Step 4 — 目录结构规划

在 `Source/WuxiaCore/` 下创建以下子目录结构：

```
Source/WuxiaCore/
├── Core/              # 核心基类、游戏模式、游戏实例
├── Character/         # 角色基类、属性集、角色数据
├── Combat/            # 战斗系统（网格、回合管理、行动）
├── Ability/           # GAS 能力、效果、武功定义
├── World/             # 大地图探索、场景切换
├── NPC/               # NPC 基类、AI、对话
├── Dialogue/          # 对话系统、对话数据
├── Inventory/         # 背包、装备、物品
├── UI/                # 所有 UMG Widget
│   ├── HUD/
│   ├── Combat/
│   ├── Dialogue/
│   └── Menu/
├── Data/              # DataAsset/DataTable 定义
└── Save/              # 存档系统
```

---

## 第二阶段：核心框架类

### Step 5 — 游戏框架基类

- `AWuxiaGameMode` — 继承 `AGameModeBase`，管理全局游戏状态（探索/战斗/对话模式切换）
- `UWuxiaGameInstance` — 继承 `UGameInstance`，持久化数据（队伍、背包、进度），跨关卡保持
- `AWuxiaPlayerController` — 继承 `APlayerController`，管理输入模式切换（探索/战斗/UI）和 45 度等轴测摄像机
- `AWuxiaHUD` — HUD 管理器

在 `Config/DefaultEngine.ini` 中将 `GameDefaultMap` 指向自建测试关卡。

### Step 6 — 角色系统

- `AWuxiaCharacterBase` — 继承 `ACharacter`，但渲染组件使用 Billboard 纹理平面（非骨骼网格体）
  - 包含 `UAbilitySystemComponent` + `UWuxiaAttributeSet`
  - 方向系统：根据移动方向切换精灵纹理（上/下/左/右 4 方向）
  - Billboard 材质：Unlit + **Nearest 纹理过滤**（保持像素风清晰），使用 `CameraPosition` 节点让精灵始终面向摄像机
- `UWuxiaAttributeSet` — GAS 属性集，定义武侠核心属性：
  - 生命（HP）、内力（MP）、体力
  - 攻击、防御、轻功（速度/闪避）、根骨（成长资质）
  - 各武功熟练度
- `UWuxiaCharacterData` — `UPrimaryDataAsset`，数据驱动角色配置：
  - 姓名、立绘、精灵图集
  - 基础属性值、成长曲线
  - 可学武功列表
  - 加入条件

### Step 7 — 精灵渲染方案（HD2D 核心）

- 创建 `M_SpriteBase` 母材质（Material）：
  - Unlit 或 Custom Lighting 模式
  - 纹理参数（精灵图集）+ 点采样（Nearest）
  - Flipbook UV 动画参数（行/列/帧索引）
  - 双面渲染 + 透明遮罩（Alpha Cutoff）
  - 可接受简化阴影（可选的自定义阴影接受逻辑）
- 创建 `UWuxiaSpriteComponent` 继承 `UStaticMeshComponent`：
  - 自动朝向摄像机（Billboard 行为）
  - Flipbook 动画播放控制（Idle/Walk/Attack/Hit/Dead）
  - 方向切换（根据角色朝向选择图集行）

---

## 第三阶段：战斗系统（金庸群侠传式）

### Step 8 — 战斗网格系统

- `AWuxiaBattleGrid` — 战斗地图 Actor：
  - 可配置的 N×M 网格（如 16×12）
  - 每个格子有地形类型（平地/障碍/水/高地）
  - 网格可视化（高亮可移动/可攻击范围）
  - 寻路：A* 或 UE5 NavMesh 适配网格
- `FBattleGridCell` — 格子数据结构（坐标、地形、占据角色）

### Step 9 — 回合管理器

- `AWuxiaBattleManager` — 战斗核心控制器：
  - **战斗状态机**：Init → TurnOrder → PlayerTurn → EnemyTurn → CheckResult → Victory/Defeat
  - **行动顺序**：按轻功（速度）属性排序，生成回合队列
  - **玩家回合**：选择角色 → 移动（网格寻路，显示可移动范围）→ 行动（攻击/武功/物品/防御/逃跑）
  - **敌人回合**：AI 控制（简单状态机：接近目标 → 攻击/使用技能）
  - 战斗结束判定：我方/敌方全灭、特殊条件
- `UWuxiaBattleTurnComponent` — 挂载在角色上，追踪当前回合状态和可用行动点

### Step 10 — 武功/技能系统（GAS 集成）

- `UWuxiaGameplayAbility` — 继承 `UGameplayAbility`，武功基类：
  - 攻击范围（十字/菱形/直线/全屏，由 DataAsset 配置）
  - 内力消耗、冷却
  - 伤害计算：攻击 × 武功威力 × 熟练度加成 − 防御 × 防御系数
  - 附加效果（中毒/眩晕/封穴 via `GameplayEffect`）
- `UWuxiaGameplayEffect` — 各种 Buff/Debuff（持续伤害、属性增减、状态异常）
- `UWuxiaAbilityData` — `UDataAsset`，武功数据配置表：
  - 名称、描述、图标
  - 攻击范围模板、内力消耗
  - 伤害类型（外功/内功）、基础威力
  - 附加效果及概率
  - 学习条件（需要特定属性值或事件触发）

### Step 11 — 战斗 UI

- `UWuxiaBattleHUD` — 战斗主界面 Widget：
  - 行动菜单（移动/攻击/武功/物品/防御/逃跑）
  - 角色状态栏（HP/MP/状态图标）
  - 回合顺序显示
  - 网格高亮反馈（可移动范围蓝色、攻击范围红色）
  - 伤害数字弹出

---

## 第四阶段：开放世界探索

### Step 12 — 等轴测摄像机系统

- `AWuxiaCameraActor` 或 `UWuxiaCameraComponent`：
  - 固定 45 度俯视角（Pitch ≈ -45°，可微调）
  - 正交投影或低 FOV 透视投影（模拟 HD2D 视角）
  - 平滑跟随玩家，支持区域锁定
  - 探索/战斗模式不同摄像机行为

### Step 13 — 大地图（World Map）

- 使用 World Partition 管理开放世界：
  - 调整 Cell Size 和 Loading Range 适配等轴测可视范围
  - Streaming Source 绑定玩家位置
- 大地图结构：Tile 化 3D 场景（地面用低多边形 + 像素风纹理），角色在上方移动
- 场景区域触发器：进入城镇区域 → 切换到城镇关卡（Level Streaming 或 World Partition Cell）
- 随机/固定遭遇触发器 → 进入战斗

### Step 14 — 城镇场景

- 垂直切片目标：构建一个完整城镇（如"临安城"）
  - 3D 场景：房屋、街道、树木（低面数 + 像素风材质）
  - NPC 精灵放置在场景中
  - 可交互物体（宝箱、门、告示牌）

### Step 15 — NPC 与对话系统

- `AWuxiaNPC` — NPC Actor，包含精灵组件 + 交互触发器
- `UWuxiaDialogueData` — `UDataAsset`，对话数据：
  - 对话节点树（节点 ID、说话人、文本、选项分支）
  - 条件分支（基于队伍成员、物品、进度标志）
  - 结果触发（给物品、加入队伍、触发战斗、开启任务）
- `UWuxiaDialogueWidget` — 对话 UI：
  - 头像 + 姓名 + 文本（逐字显示）
  - 选项列表
  - 支持 Enhanced Input 和手柄操作

---

## 第五阶段：物品与背包

### Step 16 — 物品系统

- `UWuxiaItemData` — `UDataAsset`，物品定义：
  - 类型枚举：武器/防具/消耗品/秘籍/关键物品
  - 属性加成（装备类）、使用效果（消耗品类，通过 `GameplayEffect`）
  - 图标、描述文本
- `UWuxiaInventoryComponent` — 挂载在 `GameInstance` 上的背包组件：
  - 物品列表（ID + 数量）
  - 装备栏（武器/头/身/脚/饰品）
  - 金钱

### Step 17 — 背包/装备 UI

- 背包 Widget：物品列表、分类筛选、使用/装备/丢弃
- 装备 Widget：角色纸娃娃（精灵 + 装备槽位）、属性变化对比

---

## 第六阶段：存档与流程

### Step 18 — 存档系统

- `UWuxiaSaveGame` — 继承 `USaveGame`：
  - 队伍成员列表及各自属性/装备/武功
  - 背包数据
  - 当前位置（关卡 + 坐标）
  - 剧情进度标志（`TMap<FGameplayTag, bool>`）
- 支持多个存档槽

### Step 19 — 垂直切片游戏流程

设计一个 15-30 分钟的可玩流程：
1. 主角出现在城镇（临安城），等轴测视角
2. 与 NPC 对话了解情况，获得任务
3. 在城镇内招募 1-2 名同伴
4. 离开城镇，进入大地图
5. 在大地图遭遇敌人，触发第一场战斗
6. 战斗胜利后获得经验/物品
7. 切片结束

---

## 第七阶段：数据驱动基础设施

### Step 20 — DataTable / DataAsset 配置

所有游戏内容（角色、武功、物品、对话、敌人）均由数据资产驱动，不写死在代码中：
- 在 `Content/Data/` 下建立目录：`Characters/`、`Abilities/`、`Items/`、`Dialogues/`、`Enemies/`
- 垂直切片所需最小数据量：
  - 3-4 个角色（主角 + 2-3 可招募伙伴）
  - 8-10 个武功（每人 2-3 个）
  - 10+ 物品（武器、药品、秘籍）
  - 3-5 段对话
  - 2-3 种敌人

---

## 验证方式

| 验证项 | 方法 |
|--------|------|
| HD2D 渲染效果 | 在编辑器中放置 3D 场景 + 精灵角色，确认像素风清晰、景深效果正确、Bloom 柔光自然 |
| 等轴测摄像机 | PIE 中角色移动时摄像机平滑跟随，45 度视角稳定 |
| 精灵 Billboard | 角色始终面向摄像机，Flipbook 动画流畅，方向切换正确 |
| 战斗流程 | 从遭遇触发 → 进入战斗网格 → 完成一个完整回合（移动+攻击） → 战斗结束 → 回到大地图 |
| GAS 流程 | 使用武功消耗内力、造成伤害、附加 Buff/Debuff，属性正确变化 |
| 对话系统 | 与 NPC 交互 → 对话 UI 弹出 → 选择分支 → 触发结果（加入队伍/获得物品） |
| 存档 | 存档 → 退出 → 读档，队伍/背包/位置完全恢复 |
| 完整流程 | 从头到尾走完垂直切片的 15-30 分钟流程无崩溃 |

---

## 关键技术决策

- **精灵渲染**：选择自定义 Billboard 静态网格体 + Unlit 材质，而非 Paper2D（后者在 UE5 中已停止维护，与现代渲染管线兼容性差）
- **前向渲染 + MSAA**：放弃延迟渲染（Lumen/Nanite），换取像素风精灵的清晰显示
- **GAS 作为战斗能力框架**：虽然回合制不需要 GAS 的所有功能，但属性管理、效果堆叠、标签系统非常适合武侠技能的复杂度
- **数据驱动**：所有内容配置由 DataAsset 驱动，代码只定义框架，便于后续扩展全部武功/人物/剧情

---

## 建议实施顺序

按上述 Step 1→20 的顺序，**推荐优先级**为：

1. **基础设施**（Step 1-4）→ 确保项目能编译运行
2. **精灵渲染 + 摄像机**（Step 7, 12）→ 验证 HD2D 视觉效果
3. **角色框架**（Step 5-6）→ 角色能在场景中移动
4. **战斗系统**（Step 8-11）→ 核心玩法验证
5. **探索/城镇/NPC**（Step 13-15）→ 串联场景
6. **物品/存档/流程**（Step 16-19）→ 完成切片
7. **数据填充**（Step 20）→ 填入具体内容
