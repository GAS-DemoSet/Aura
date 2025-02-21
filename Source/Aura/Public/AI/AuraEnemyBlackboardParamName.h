#pragma once

struct FAuraEnemyBlackboardParamName
{
public:
	/** SelfActor */
	static const FName SelfActor;
	
	/** TargetToFollow 追击/打击目标 */
	static const FName TargetToFollow;

	/** DistanceToTarget 距离目标的距离，也就是TargetToFollow的距离 */
	static const FName DistanceToTarget;
	
	/** bHitReacting 当怪物被打击时的状态 */
	static const FName HitReacting;

	/** bRangedAttacker 是否为远程攻击者 */
	static const FName RangedAttacker;
};
