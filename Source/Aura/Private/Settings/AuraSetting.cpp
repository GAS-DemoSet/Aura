


#include "Settings/AuraSetting.h"

const UAuraSetting& UAuraSetting::Get()
{
	const UAuraSetting* Setting = GetDefault<UAuraSetting>();
	check(Setting);
	return *Setting;
}

bool UAuraSetting::GetDrawDebug_GAS()
{
	return UAuraSetting::Get().bDrawDebug_GAS;
}
