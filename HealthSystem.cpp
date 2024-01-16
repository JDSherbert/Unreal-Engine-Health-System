// HealthAndDamageComponent.cpp

#include "HealthAndDamageComponent.h"

// Sets default values for this component's properties
UHealthAndDamageComponent::UHealthAndDamageComponent(float InitialHealth, float InitialMaxHealth)
    : Health(InitialHealth), MaxHealth(FMath::Max(0.0f, InitialMaxHealth))
{
    // Set this component to be initialized when the game starts
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthAndDamageComponent::TakeDamage(float DamageAmount, EDamageType DamageType)
{
    // Check for damage resistance and weakness
    float ModifiedDamageAmount = DamageAmount;

    if (DamageModifiers.DamageResistances.Contains(DamageType))
    {
        ModifiedDamageAmount *= (1.0f - DamageModifiers.DamageResistances[DamageType]);
    }

    if (DamageModifiers.DamageWeaknesses.Contains(DamageType))
    {
        ModifiedDamageAmount *= (1.0f + DamageModifiers.DamageWeaknesses[DamageType]);
    }

    // Apply modified damage to health
    Health -= ModifiedDamageAmount;

    // Trigger the OnHealthChanged event
    NotifyHealthChanged();

    // Check if the entity is dead
    if (Health <= 0)
    {
        // Trigger the OnDeath event
        OnDeath.Broadcast(GetOwner(), Health);
    }

    // Implement any other necessary damage-related logic
}

void UHealthAndDamageComponent::Heal(float HealAmount)
{
    // Calculate the new health after healing
    float NewHealth = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);

    // Calculate the actual amount healed
    float ActualHealAmount = NewHealth - Health;

    // Update health
    Health = NewHealth;

    // Trigger the OnHealthChanged event
    NotifyHealthChanged();

    // Implement any other necessary healing-related logic
}

void UHealthAndDamageComponent::SetDamageModifiers(const FDamageModifiers& Modifiers)
{
    DamageModifiers = Modifiers;
}

void UHealthAndDamageComponent::SetHealth(float NewHealth)
{
    // Set the new health value within the valid range
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    // Trigger the OnHealthChanged event
    NotifyHealthChanged();
}

void UHealthAndDamageComponent::SetMaxHealth(float NewMaxHealth)
{
    // Update the max health value
    MaxHealth = FMath::Max(0.0f, NewMaxHealth);

    // Ensure that current health is within the updated max health
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);

    // Trigger the OnHealthChanged event
    NotifyHealthChanged();
}

void UHealthAndDamageComponent::NotifyHealthChanged()
{
    // Trigger the OnHealthChanged event with the new health value
    OnHealthChanged.Broadcast(Health);
}
