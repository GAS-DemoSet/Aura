#include "AbilitySystem/ExecCalc/EC_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interface/CombatInterface.h"

// 定义 Armor 变量以及 Armor 的属性捕获参数
struct AuraDamageStatics
{
	// 目标护甲值
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	// 目标阻挡几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	// 目标暴击抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	// 源对象护甲穿透
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	// 源对象暴击率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	// 源对象暴击伤害
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	
	AuraDamageStatics()
	{
		// 捕获目标对象属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		// 捕获源对象属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics Ds;
	return Ds;
}

UEC_Damage::UEC_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

// 防优化宏
PRAGMA_DISABLE_OPTIMIZATION
void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);

	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();
	
	// TODO:: 计算...
	
	// float Armor = 0.f;
	// 参数捕获
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, Armor);
	// Armor = FMath::Max<float>(0.f, Armor);
	// Armor++;

	// 提交修改
	// FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	// OutExecutionOutput.AddOutputModifier(EvaluatedData);

	// Get damage set by caller Magnitude
	// 获取由源对象设置的伤害值
	float Damage = 0.f;
	for (const auto& Iter : FAuraGameplayTags::Get()->DamageTypes)
	{
		const float TempDamageTypeVal = GESpec.GetSetByCallerMagnitude(Iter);
		Damage += TempDamageTypeVal;
	}

	// 捕获目标上的 阻挡几率，并确定是否成功阻止 如果阻止，则将伤害减半。
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);
	// 几率测算
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = GESpec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	// 如果阻止，则将伤害减半。
	Damage = bBlocked ? Damage / 2.f : Damage;

	// 计算穿甲值
	// 捕捉 目标 护甲 值
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max(TargetArmor, 0.f);

	// 捕捉捕获 伤害源 的 护甲穿透 值
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);

	// 获取伤害计算曲线表
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	checkf(CharacterClassInfo->DamageCalculationCoefficients, TEXT("UEC_Damage::Execute_Implementation:: !!!"));

	// 获取穿甲系数曲线
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	check(ArmorPenetrationCurve);
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	// 获取有效装甲系数曲线
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	check(EffectiveArmorCurve);
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// 穿甲值忽略一部分护甲
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	// 护甲值忽略一部分伤害
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// 获取源对象暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.f);

	// 获取源对象暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.f);

	// 获取目标对象暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.f);

	// 获取暴击抗性系数曲线
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	check(CriticalHitResistanceCurve);
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// 暴击抗性减少一定得暴击几率
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// 双倍伤害加暴击加成
	Damage = bCriticalHit ? 2 * Damage + SourceCriticalHitDamage : Damage;

	// 提交修改
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
// 防优化宏
PRAGMA_ENABLE_OPTIMIZATION
