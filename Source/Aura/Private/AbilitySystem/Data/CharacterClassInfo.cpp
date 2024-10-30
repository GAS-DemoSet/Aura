// 


#include "AbilitySystem/Data/CharacterClassInfo.h"

const FCharacterClassDefaultInfo& UCharacterClassInfo::FindClassDefaultInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
