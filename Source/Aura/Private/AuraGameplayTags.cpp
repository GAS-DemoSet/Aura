#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

TSharedPtr<FAuraGameplayTags> FAuraGameplayTags::GameplayTags = nullptr;

TSharedPtr<FAuraGameplayTags> FAuraGameplayTags::Get()
{
	if (!GameplayTags.IsValid())
	{
		GameplayTags = MakeShareable(new(FAuraGameplayTags));
	}
	checkf(GameplayTags, TEXT("Error:: FAuraGameplayTags::Get() GameplayTags is null!!!"));
	return GameplayTags;
}

void FAuraGameplayTags::Clear()
{
	if (GameplayTags.IsValid())
	{
		GameplayTags->UnInitializeNativeGameplayTags();
		GameplayTags.Reset();
	}
	GameplayTags = nullptr;
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage（主要属性：力量值）")
		);

	Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage（主要属性：智力值）")
		);

	Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration（主要属性：韧性值）")
		);

	Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health（主要属性：活力值）")
		);

	
	/*
	 * Secondary Attributes
	 */
	Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance（次要属性：护甲值）")
		);

	Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance（次要属性：护甲穿透值）")
		);

	Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half（次要属性：阻挡几率值）")
		);

	Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus（次要属性：暴击率值）")
		);

	Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored（次要属性：暴击伤害值）")
		);

	Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies（次要属性：暴击抗性值）")
		);

	Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second（次要属性：生命恢复值）")
		);

	Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second（次要属性：法力恢复值）")
		);

	Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable（次要属性：最大生命值）")
		);

	Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable（次要属性：最大法力值）")
		);


	/*
	 * Input Tags
	 */
	InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button（鼠标左键）")
		);

	InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button（鼠标右键）")
		);

	InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
		);

	InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
		);

	InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
		);

	InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
		);

	InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input Tag Passive Ability 1")
		);

	InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Tag Passive Ability 2")
		);


	/*
	 * Damage Types
	 */
	Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage（伤害）")
		);
	Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type（火焰伤害）")
		);
	Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type（雷电伤害）")
		);
	Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type（奥术伤害）")
		);
	Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type（物理伤害）")
		);

	
	/**
	 * 次要属性：伤害抵抗标签
	 */
	Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Fire"),
		FString("Resistance（火焰抵抗）")
		);

	Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Lightning"),
		FString("Resistance（雷电抵抗）")
		);

	Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Arcane"),
		FString("Resistance（奥术抵抗）")
		);

	Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Physical"),
		FString("Resistance（物理抵抗）")
		);

	/**
	 * Map of Damage to Resistance
	 */
	DamageTypesToResistances.Add(Damage_Fire, Attributes_Resistance_Fire);
	DamageTypesToResistances.Add(Damage_Lightning, Attributes_Resistance_Lightning);
	DamageTypesToResistances.Add(Damage_Arcane, Attributes_Resistance_Arcane);
	DamageTypesToResistances.Add(Damage_Physical, Attributes_Resistance_Physical);

	/**
	 * Ability Tags
	 */
	Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("Hit React Ability")
		);
}

void FAuraGameplayTags::UnInitializeNativeGameplayTags()
{
}
