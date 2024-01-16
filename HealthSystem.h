// HealthAndDamageComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageTypes.h"
#include "HealthAndDamageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, Owner, float, FinalHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

// Private namespace for default values
namespace HealthAndDamageDefaults
{
    constexpr float DefaultHealth = 100.0f;
    constexpr float DefaultMaxHealth = 100.0f;
}

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOURPROJECT_API UHealthAndDamageComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UHealthAndDamageComponent(float InitialHealth = HealthAndDamageDefaults::DefaultHealth, float InitialMaxHealth = HealthAndDamageDefaults::DefaultMaxHealth);

    // Event triggered when the entity dies
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

    // Event triggered when the entity's health changes
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChanged OnHealthChanged;

    // Function to apply damage to the entity
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount, EDamageType DamageType);

    // Function to restore health to the entity
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    // Function to set damage modifiers (resistances and weaknesses)
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void SetDamageModifiers(const FDamageModifiers& Modifiers);

    // Getter for current health
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const { return Health; }

    // Setter for current health (useful for initialization and validation)
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealth(float NewHealth);

    // Getter for max health
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    // Setter for max health
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetMaxHealth(float NewMaxHealth);

    // Add more functions and properties for customization based on your needs

protected:
    // Current health of the entity
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float Health;

    // Maximum health of the entity
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
    float MaxHealth;

    // Damage modifiers for resistances and weaknesses
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
    FDamageModifiers DamageModifiers;

    // Helper function to trigger the OnHealthChanged event
    void NotifyHealthChanged();
};
