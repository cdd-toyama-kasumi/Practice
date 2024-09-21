#pragma once

#include "Logging/LogMacros.h"
///////////////////////////// Log Helpers //////////////////////////////////////

// Default category used in `UE_LOG`.
#define DEFAULT_LOG_CATEGORY LogTemp

// e.g. AActor::BeginPlay() => <Your Message> ["\Path\To\Actor.cpp:299"]
//EXPAND UE_LOG(LogTemp, XX, TEXT("%hs() => ") TEXT("%hs") TEXT("%hs->%d"), __FUNCTION__, "Construct" ,__FILE__, __LINE__)
#define DEFAULT_LOG_MESSAGE(RawMessage) TEXT("%hs() => ") TEXT(RawMessage) TEXT(" [\"%hs:%d\"]")

// __FUNCTION__, Type: %hs, e.g.: AActor::BeginPlay
// __FILE__,     Type: %hs, e.g.: \Path\To\Actor.cpp
// __LINE__,     Type: %d,  e.g.: 299
// __VA_OPT__(,) (since cpp20): if `...` is empty, leave it empty as well, otherwise add a `,` to separate nearby tokens.

#define DEFAULT_LOG_ARGUMENTS(...) __FUNCTION__ __VA_OPT__(,) __VA_ARGS__, __FILE__, __LINE__

// Unreal Assertions
#define LogCheck(Condition, Format, ...)	checkf(Condition,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogCheckSlow(Condition, Format, ...)	checkfSlow(Condition,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogVerify(Condition, Format, ...)	verifyf(Condition,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogEnsure(Condition, Format, ...)	ensureMsgf(Condition,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))

// Unreal Logs
#define LogVerbose(Format, ...) UE_LOG(DEFAULT_LOG_CATEGORY, Verbose,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogDisplay(Format, ...) UE_LOG(DEFAULT_LOG_CATEGORY, Display,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogDefault(Format, ...) UE_LOG(DEFAULT_LOG_CATEGORY, Log,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogWarning(Format, ...) UE_LOG(DEFAULT_LOG_CATEGORY, Warning,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogError(Format, ...)	UE_LOG(DEFAULT_LOG_CATEGORY, Error,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))
#define LogFatal(Format, ...)	UE_LOG(DEFAULT_LOG_CATEGORY, Fatal,	DEFAULT_LOG_MESSAGE(Format), DEFAULT_LOG_ARGUMENTS(__VA_ARGS__))

// Assertion helpers
#define CheckValid(VarMaybeNull)		LogCheck(VarMaybeNull, "`" #VarMaybeNull "` is nullptr!")
#define CheckInvalid(VarMaybeValid)		LogCheck(VarMaybeValid, "`" #VarMaybeValid "` is valid!")
#define CheckTrue(VarMaybeFalse)		LogCheck(VarMaybeFalse, "`" #VarMaybeFalse "` is false!")
#define CheckFalse(VarMaybeTrue)		LogCheck(VarMaybeTrue, "`" #VarMaybeTrue "` is true!")
#define CheckEqual(Var1, Var2)			LogCheck(Var1 == Var2, "`" #Var1 "` != `" #Var2 "`!")
#define CheckNotEqual(Var1, Var2)		LogCheck(Var1 != Var2, "`" #Var1 "` == `" #Var2 "`!")
#define EnsureValid(VarMaybeNull)		LogEnsure(VarMaybeNull, "`" #VarMaybeNull "` is nullptr!")
#define EnsureInvalid(VarMaybeValid)		LogEnsure(VarMaybeValid, "`" #VarMaybeValid "` is valid!")
#define EnsureTrue(VarMaybeFalse)		LogEnsure(VarMaybeFalse, "`" #VarMaybeFalse "` is false!")
#define EnsureFalse(VarMaybeTrue)		LogEnsure(VarMaybeTrue, "`" #VarMaybeTrue "` is true!")
#define EnsureEqual(Var1, Var2)			LogEnsure(Var1 == Var2, "`" #Var1 "` != `" #Var2 "`!")
#define EnsureNotEqual(Var1, Var2)		LogEnsure(Var1 != Var2, "`" #Var1 "` == `" #Var2 "`!")

///////////////////////////// Log Helpers ///////////////