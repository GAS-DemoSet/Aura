// 


#include "AuraAssetManager.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"
#include "Log/AuraLog.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("UAuraAssetManager::Get():: GEngine is nullptr!!!"));
	if (!AuraAssetManager.IsValid())
	{
		AuraAssetManager = CastChecked<UAuraAssetManager>(GEngine->AssetManager);
	}
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(AuraLog, Warning, TEXT("UAuraAssetManager::StartInitialLoading()"));

	// 注册 GameplayTag
	FAuraGameplayTags::Get()->InitializeNativeGameplayTags();

	// 5.3不需要再次手动调用了,引擎在模块初始化时已经调用过了（GameplayAbilitiesModule.cpp）（版本若低时依旧需要主动调用）
	// UAbilitySystemGlobals::Get().InitGlobalData();
}

void UAuraAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();

	UE_LOG(AuraLog, Warning, TEXT("UAuraAssetManager::FinishInitialLoading()"));
}

void UAuraAssetManager::FinishDestroy()
{
	Super::FinishDestroy();

	UE_LOG(AuraLog, Warning, TEXT("UAuraAssetManager::FinishDestroy()"));

	FAuraGameplayTags::Clear();
}
