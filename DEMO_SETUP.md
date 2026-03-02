# 武侠 HD2D — 最小 Demo 搭建指南

## 前置条件

- Unreal Engine 5.7 已安装
- Visual Studio 2022 或 Rider（C++ 编译）
- 项目已 clone 到本地

---

## 第 1 步：编译项目

### 方式 A：IDE 编译（推荐）

1. 右键 `WuxiaCore.uproject` → **Generate Visual Studio project files**
2. 打开生成的 `WuxiaCore.sln`
3. 配置选择 **Development Editor | Win64**
4. `Ctrl+B` 编译

### 方式 B：编辑器自动编译

1. 双击 `WuxiaCore.uproject` 打开 UE 编辑器
2. 首次打开会自动提示编译，点 **Yes**
3. 等待编译完成（底部进度条）

---

## 第 2 步：创建测试关卡

1. 打开 UE 编辑器
2. **File → New Level → Empty Level**
3. `Ctrl+S` 保存 → 路径填 `Content/Maps/TestSlice`
   - 保存后引擎内路径为 `/Game/Maps/TestSlice`
   - 与 `Config/DefaultEngine.ini` 中的 `GameDefaultMap` 配置一致

---

## 第 3 步：搭建基础场景

在关卡视口中依次放置以下 Actor：

### 3.1 地面

1. **Place Actors** 面板 → 搜索 `Cube`
2. 拖入场景
3. Details 面板设置：
   - **Location**: `(0, 0, -50)`
   - **Scale**: `(50, 50, 1)`

### 3.2 光源

1. **Place Actors** → 搜索 `Directional Light`
2. 拖入场景
3. Details 面板设置：
   - **Rotation**: `(-45, -45, 0)`
   - **Intensity**: `3.14`（默认即可）

### 3.3 天空

1. **Place Actors** → 搜索 `Sky Atmosphere` → 拖入
2. **Place Actors** → 搜索 `Sky Light` → 拖入
   - 勾选 **Real Time Capture**

### 3.4 Player Start

1. **Place Actors** → 搜索 `Player Start`
2. 拖到地面上方
3. **Location**: `(0, 0, 100)`

---

## 第 4 步：设置 GameMode

1. 工具栏 → **Settings → World Settings**
2. 在 **World Settings** 面板中找到 **GameMode Override**
3. 选择 `WuxiaGameMode`
4. 确认下方自动填充为：

| 属性 | 值 |
|------|----|
| Default Pawn Class | `WuxiaExplorationPawn` |
| Player Controller Class | `WuxiaPlayerController` |
| HUD Class | `WuxiaHUD` |

> 如果没有出现 `WuxiaGameMode`，说明编译未成功，回到第 1 步。

---

## 第 5 步：运行测试

按下 **Alt+P** 或点击工具栏 ▶️ **Play**。

### 预期效果

| 功能 | 预期表现 |
|------|---------|
| 摄像机 | 45° 等轴测俯视角 |
| 角色 | 位置出现白色平面（精灵占位符） |
| WASD 移动 | 角色在地面上滑动 |
| E 键交互 | 控制台输出交互日志 |
| Billboard | 白色平面始终面向摄像机 |

### 操控说明

| 按键 | 功能 |
|------|------|
| `W` | 向前移动 |
| `S` | 向后移动 |
| `A` | 向左移动 |
| `D` | 向右移动 |
| `E` | 交互（需靠近 NPC） |

> 输入映射已在 `WuxiaPlayerController` 中通过代码自动创建，无需配置 InputAction 资产。

---

## 可选增强

完成基础 demo 后，按以下顺序逐步增强：

### A. 添加 HD2D 后处理效果

1. **Place Actors** → 搜索 `Post Process Volume` → 拖入场景
2. Details 面板：
   - 勾选 **Infinite Extent (Unbound)**
   - **Bloom → Intensity**: `0.4`
   - **Depth of Field → Focal Distance**: `1500`（与弹簧臂距离一致）
   - **Depth of Field → Aperture (F-stop)**: `2.0`
   - **Color Grading → Saturation**: 稍微降低到 `0.85` 获得水墨感

### B. 创建精灵材质 M_SpriteBase

1. Content Browser → 右键 → **Material** → 命名 `M_SpriteBase`
2. 双击打开材质编辑器
3. 设置：
   - **Shading Model**: `Unlit`
   - **Blend Mode**: `Masked`
   - **Two Sided**: ✅
4. 添加节点：
   - `TextureSampleParameter2D` → 命名 `SpriteTexture`
   - `ScalarParameter` × 4 → 分别命名 `UOffset`, `VOffset`, `UScale`, `VScale`
5. UV 计算连线：
   ```
   TexCoord → Multiply(UScale, VScale) → Add(UOffset, VOffset) → SpriteTexture.UV
   SpriteTexture.RGB → Emissive Color
   SpriteTexture.A → Opacity Mask
   ```
6. 保存后在蓝图或代码中调用 `SpriteComp->SetSpriteMaterial(M_SpriteBase)`

### C. 导入占位精灵图

1. 从 [itch.io](https://itch.io/game-assets/tag-pixel-art) 下载 RPG 角色 spritesheet
2. 拖入 `Content/Textures/Characters/`
3. 双击纹理 → **Filter**: `Nearest`（像素风关键设置）
4. 在角色 DataAsset 中指定 SpriteSheet 引用

### D. 放置测试 NPC

1. **Place Actors** → 搜索 `WuxiaNPC` → 拖入场景
2. Details 面板：
   - **NPC Name**: `"测试NPC"`
3. 运行后走近 NPC → 按 `E` → 查看控制台输出

---

## 项目结构参考

```
WuxiaCore.uproject
Config/
    DefaultEngine.ini          ← 渲染设置 + 默认地图
Source/WuxiaCore/
    Core/
        WuxiaGameMode          ← 游戏模式（探索/战斗切换）
        WuxiaPlayerController  ← 输入控制（自带 WASD 默认映射）
        WuxiaGameInstance      ← 队伍/背包/进度持久化
        WuxiaHUD               ← HUD 管理器
    Character/
        WuxiaCharacterBase     ← 角色基类（GAS + 精灵）
        WuxiaSpriteComponent   ← ProceduralMesh Billboard 精灵
        WuxiaAttributeSet      ← GAS 属性（生命/内力/攻防等）
        WuxiaCharacterData     ← 角色 DataAsset
    World/
        WuxiaExplorationPawn   ← 可操控角色（弹簧臂 + 摄像机）
        WuxiaCameraComponent   ← 等轴测摄像机
    Combat/
        WuxiaBattleGrid        ← 战斗网格（BFS 寻路）
        WuxiaBattleManager     ← 回合制战斗状态机
        WuxiaBattleTurnComponent
    NPC/
        WuxiaNPC               ← NPC（精灵 + 交互触发器）
    Ability/
        WuxiaGameplayAbility   ← GAS 技能基类
        WuxiaAbilityData       ← 技能 DataAsset
    Dialogue/
        WuxiaDialogueData      ← 对话数据资产
        WuxiaDialogueComponent ← 对话控制器
    Inventory/
        WuxiaItemData          ← 物品 DataAsset
        WuxiaInventoryComponent← 背包系统
    UI/
        WuxiaBattleHUDWidget   ← 战斗 UI
        WuxiaDialogueWidget    ← 对话 UI
        WuxiaMainHUDWidget     ← 主界面 UI
        WuxiaInventoryWidget   ← 背包 UI
    Save/
        WuxiaSaveGame          ← 存档系统
    Data/
        WuxiaDataTypes         ← 全局枚举/结构体
```

---

## 常见问题

### Q: 编译报错 `ProceduralMeshComponent.h not found`
A: 确认 `WuxiaCore.uproject` 中已启用 `ProceduralMeshComponent` 插件，且 `WuxiaCore.Build.cs` 的依赖中包含 `"ProceduralMeshComponent"`。

### Q: Play 时没有角色生成
A: 检查 World Settings 中 GameMode Override 是否设为 `WuxiaGameMode`，以及场景中是否有 `Player Start`。

### Q: WASD 无法移动
A: 确认 **Project Settings → Input → Default Classes → Default Player Input Class** 为 `EnhancedPlayerInput`，**Default Input Component Class** 为 `EnhancedInputComponent`。

### Q: 角色看不到白色平面
A: 检查控制台日志是否有 `WuxiaSpriteComponent: 未设置精灵材质` 警告。如有，说明回退材质生效但可能被遮挡。尝试调整 Player Start 位置或摄像机距离。
