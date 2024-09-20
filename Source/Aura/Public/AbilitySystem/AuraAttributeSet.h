// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	//~ Begin UAttributeSet Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//~ End UAttributeSet Interface

	/*
	 * Primary Attribute
	 */
	/** 力量 */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	/** 智力 */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	/** 韧性 */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	/** 活力 */
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attribute", ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	/*
	 * Secondary Attributes
	 */
	/** 护甲值 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	/** 护甲穿透 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	/** 阻挡几率 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	/** 暴击率 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	/** 暴击伤害 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	/** 暴击抗性 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	/** 生命恢复 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	/** 法力恢复 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	/** 最大生命值 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	/** 最大法力值 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/*
	 * Resistance Attributes
	 */
	/** 耐火性 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	/** 抗雷击 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	/** 奥术抗性 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	/** 物理阻力 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	/*
	 * Vital Attribute
	 */
	/** 当前血量 */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	/** 当前法力 */
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attribute", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	/*
	 * Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);

public:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectPro) const;
};