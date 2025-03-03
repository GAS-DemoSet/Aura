// 

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 获取主界面数据控制器 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static UOverlapWidgetController* GetOverlapWidgetController(const UObject* WorldContextObject);

	/** 获取属性界面数据控制器 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	/** 初始化怪物角色的属性 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	/** 注册怪物技能集合 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void GiveStartupAbility(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC);

	/** 获取怪物角色类型数据 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	/** 攻击是否被格挡 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/** 攻击是否带暴击 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/** 设置攻击格挡 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(Ref)FGameplayEffectContextHandle& EffectContextHandle, bool bNewBlockedHit);

	/** 设置攻击暴击 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(Ref)FGameplayEffectContextHandle& EffectContextHandle, bool bNewCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMachanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutLivePlayer, const TArray<AActor*> IgnoreActor, float Radius, const FVector& SphereOrigin);
};
