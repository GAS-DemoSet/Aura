#include "Character/AuraEnemy.h"

AAuraEnemy::AAuraEnemy()
{
}

void AAuraEnemy::HighLightActor()
{
	bHighLightDebug = true;
}

void AAuraEnemy::UnHighLightActor()
{
	bHighLightDebug = false;
}
