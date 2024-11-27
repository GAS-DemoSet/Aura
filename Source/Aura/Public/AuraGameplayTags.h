#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAuraGameplayTags
{
public:
	static TSharedPtr<FAuraGameplayTags> Get();
	static void Clear();
	
	void InitializeNativeGameplayTags();
	void UnInitializeNativeGameplayTags();

	/*
	 * Primary Attributes
	 */
	/** Increases physical damage（主要属性：力量值） */
	FGameplayTag Attributes_Primary_Strength;
	/** Increases magical damage（主要属性：智力值） */
	FGameplayTag Attributes_Primary_Intelligence;
	/** Increases Armor and Armor Penetration（主要属性：韧性值） */
	FGameplayTag Attributes_Primary_Resilience;
	/** Increases Health（主要属性：活力值） */
	FGameplayTag Attributes_Primary_Vigor;


	/*
	 * Secondary Attributes
	 */
	/** Reduces damage taken, improves Block Chance（次要属性：护甲值） */
	FGameplayTag Attributes_Secondary_Armor;
	/** Ignores Percentage of enemy Armor, increases Critical Hit Chance（次要属性：护甲穿透值） */
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	/** Chance to cut incoming damage in half（次要属性：阻挡几率值） */
	FGameplayTag Attributes_Secondary_BlockChance;
	/** Chance to double damage plus critical hit bonus（次要属性：暴击率值） */
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	/** Bonus damage added when a critical hit is scored（次要属性：暴击伤害值） */
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	/** Reduces Critical Hit Chance of attacking enemies（次要属性：暴击抗性值） */
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	/** Amount of Health regenerated every 1 second（次要属性：生命恢复值） */
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	/** Amount of Mana regenerated every 1 second（次要属性：法力恢复值） */
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	/** Maximum amount of Health obtainable（次要属性：最大生命值） */
	FGameplayTag Attributes_Secondary_MaxHealth;
	/** Maximum amount of Mana obtainable（次要属性：最大法力值） */
	FGameplayTag Attributes_Secondary_MaxMana;

	/*
	 * Input Tags
	 */
	/** Input Tag for Left Mouse Button（鼠标左键） */
	FGameplayTag InputTag_LMB;
	/** Input Tag for Right Mouse Button（鼠标右键） */
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	/*
	 * Damage Types
	 */
	FGameplayTag Damage;
	/** Fire Damage Type（火焰伤害） */
	FGameplayTag Damage_Fire;
	/** Lightning Damage Type（雷电伤害） */
	FGameplayTag Damage_Lightning;
	/** Arcane Damage Type（奥术伤害） */
	FGameplayTag Damage_Arcane;
	/** Physical Damage Type（物理伤害） */
	FGameplayTag Damage_Physical;

	/** 伤害类型集合 */
	TArray<FGameplayTag> DamageTypes;
	
	/*
	 * Ability
	 */
	FGameplayTag Effect_HitReact;

private:
	static TSharedPtr<FAuraGameplayTags> GameplayTags;
};
