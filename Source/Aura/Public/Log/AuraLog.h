#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"

DECLARE_LOG_CATEGORY_EXTERN(AuraLog, Log, All);

#define Print_Log_NetRole(InActor, Verbosity, Log) \
if (InActor) \
{ \
	if (const UEnum* EnumObject = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"))) \
	{ \
		const FName EnumName = EnumObject->GetNameByValue(static_cast<int64>(InActor->GetLocalRole())); \
		UE_LOG(AuraLog, Verbosity, TEXT("%s[%d]: [%s]: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__, *EnumName.ToString(), Log); \
	} \
} else { \
	UE_LOG(AuraLog, Verbosity, TEXT("%s[%d]: [传入的Actor为空]: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__, Log); \
}

#define Print_Log(Verbosity, Log) \
UE_LOG(AuraLog, Verbosity, TEXT("%s[%d]: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__, Log); 
