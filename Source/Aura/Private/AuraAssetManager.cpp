// 


#include "AuraAssetManager.h"

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
