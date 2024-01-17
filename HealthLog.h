// ©2024 JDSherbert. All rights reserved.

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include "HealthLog.generated.h"

/* ------------------------------ Struct Definition ------------------------------- */

USTRUCT(BlueprintType)
struct SHERBERT_API FHealthLog
{
    GENERATED_BODY()
    
    FHealthLog(FName NewSource, float NewValue, FString NewTimestamp)
    : Source(NewSource)
    , Value(NewValue)
    , Timestamp(NewTimestamp)
    {
    }
    
    /** Name of the actor causing the damage. */
    UPROPERTY(BlueprintReadOnly, Category = "Sherbert|Data")
    FName Source;

    /** Damage or healing value (positive for healing, negative for damage) */
    UPROPERTY(BlueprintReadOnly, Category = "Sherbert|Data")
    float Value;

    /** Timestamp of when the damage or healing was inflicted */
    UPROPERTY(BlueprintReadOnly, Category = "Sherbert|Data"
    FString Timestamp;
};

/* ------------------------------------------------------------------------------- */
