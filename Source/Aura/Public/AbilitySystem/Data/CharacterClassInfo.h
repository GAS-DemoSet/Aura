// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UAuraGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	/** 元素师 */
	ECC_Elementalist,

	/** 战士 */
	ECC_Warrior,

	/** 游侠 */
	ECC_Ranger,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	/** 对应 */
	UPROPERTY(EditDefaultsOnly, Category = "Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	/** 角色技能集合 */
	UPROPERTY(EditDefaultsOnly, Category = "Class Default")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbility;

};

/**
 * 怪物角色数据
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	/** 不同角色的主要属性不同 */
	UPROPERTY(EditDefaultsOnly, Category = "Characer Class Default")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	/** 次要属性(通用) */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** 重要属性(通用) */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/** 玩家自带技能集合 */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TArray<TSubclassOf<UAuraGameplayAbility>> CommonAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	const FCharacterClassDefaultInfo& FindClassDefaultInfo(ECharacterClass CharacterClass) const;
};
