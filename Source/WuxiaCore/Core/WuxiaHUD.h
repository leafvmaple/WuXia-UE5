// WuxiaHUD.h — HUD 管理器
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WuxiaHUD.generated.h"

UCLASS()
class WUXIACORE_API AWuxiaHUD : public AHUD
{
	GENERATED_BODY()

public:
	AWuxiaHUD();

protected:
	virtual void BeginPlay() override;
};
