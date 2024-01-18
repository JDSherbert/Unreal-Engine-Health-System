// ©2024 JDSherbert. All Rights Reserved.

#include "HealthComponent.h"

/* ---------------------------- Method Definitions ------------------------------- */
 
UHealthComponent::UHealthComponent(float InitialHealth, float InitialMaxHealth)
    : Health(InitialHealth)
    , MaxHealth(InitialMaxHealth)
{
    PrimaryComponentTick.bCanEverTick = false;

    // Validate values and ensure they stay within expected ranges.
    Clamp_MaxHealth(MaxHealth);
    Clamp_CurrentHealth(CurrentHealth);
    DeathCheck();
}

/* ------------------------------------------------------------------------------- */

void UHealthComponent::InflictDamage(const float Amount)
{
    const float TempHealth = CurrentHealth;
    CurrentHealth = Clamp_CurrentHealth(CurrentHealth -= Amount);

    // Health changed?
    if(TempHealth != CurrentHealth)
    {
        Event_OnHealthChanged(CurrentHealth);
        DeathCheck();
    }
}

/* ------------------------------------------------------------------------------- */

void UHealthComponent::Heal(const float Amount)
{
    const float TempHealth = CurrentHealth;
    CurrentHealth = Clamp_CurrentHealth(CurrentHealth += Amount);

    // Health changed?
    if(TempHealth != CurrentHealth)
    {
        Event_OnHealthChanged(CurrentHealth);
        DeathCheck();
    }
}

/* ------------------------------------------------------------------------------- */
