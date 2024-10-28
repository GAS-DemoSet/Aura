

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250.f

// 定义投射组件通道，方便C++中使用（项目设置中需要添加该通道方可正常使用）
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
