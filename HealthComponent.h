// HealthAndDamageComponent.h

#pragma once

#include "CoreMinimal.h"

#include <Runtime/Engine/Classes/Components/ActorComponent.h>
#include <Runtime/Engine/Classes/GameFramework/DamageType.h>

#include "HealthComponent.generated.h"

namespace HealthDefaults
{
    constexpr float DefaultHealth = 100.0f;
    constexpr float DefaultMaxHealth = 100.0f;
}

/* ------------------------------ Class Definition ------------------------------- */

/**
* Health Component Class.
* Holds a current + max health value, fires events when health changes or becomes less than/equal to 0.0f.
* Damage is assigned via built in UDamage objects.
* Very quick easy component to add functionality to damageable objects.
* @since 16/01/2024
* @author JDSherbert
*/
UCLASS(ClassGroup = "Sherbert", Blueprintable, meta = (BlueprintSpawnableComponent))
class SHERBERT_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    // Sets default values for this component's properties
    UHealthComponent
    (
        float InitialHealth = HealthDefaults::DefaultHealth, 
        float InitialMaxHealth = HealthDefaults::DefaultMaxHealth
    );

    /**
	* Apply damage to this entity.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    void InflictDamage(float Amount);

    /**
	* Apply healing to this entity.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    void Heal(float Amount);

    /**
	* Getter method. Returns the current health.
	* @return float : Returns the current health of this component.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintPure, Category = "Sherbert|Component|Health")
    FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

    /**
	* Getter method. Returns the maximum health.
	* @return float : Returns the max health of this component.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintPure, Category = "Sherbert|Component|Health")
    FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

    /**
	* Setter method. Sets current health. 
    * Preferred function for initialization and validation. Will not allow the set value to exceed MaxHealth or be below 0.0f.
	* @param NewCurrentHealth : Sets the CurrentHealth on this component.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    FORCEINLINE void SetCurrentHealth(float NewCurrentHealth) { CurrentHealth = Clamp_CurrentHealth(NewCurrentHealth); }

    /**
	* Setter method. Sets max health. 
    * Preferred function for initialization and validation. Will not allow the set value to be below 0.0f.
    * Will also reduce CurrentHealth to match this value, if the new value is less than the CurrentHealth value.
	* @param NewMaxHealth : Sets the MaxHealth on this component, and the CurrentHealth if it overflows the max.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    FORCEINLINE void SetMaxHealth(float NewMaxHealth) { MaxHealth = Clamp_MaxHealth(NewMaxHealth); }

private:

    /* Current health of the entity. */
    UPROPERTY(Transient, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    /* Maximum health of the entity. */
    UPROPERTY(EditDefaultsOnly, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    float MaxHealth;

    /**
	* Validation method. Clamps inbound values to expected values.
	* @param Value : An expected CurrentHealth value.
	* @return float : Returns Value if valid within range, otherwise returns the expected clamped value (0.0f or MaxHealth).
	* @since 16/01/2024
	* @author JDSherbert
	*/
    FORCEINLINE float Clamp_CurrentHealth(float Value) { return FMath::Clamp(Value, 0.0f, MaxHealth); }

    /**
	* Validation method. Clamps inbound values to expected values.
	* @param Value : An expected MaxHealth value.
	* @return float : Returns Value if valid within range, otherwise returns the expected clamped value (0.0f).
	* @since 16/01/2024
	* @author JDSherbert
	*/
    FORCEINLINE float Clamp_MaxHealth(float Value) { return FMath::Max(0.0f, Value); }

public:

	/* ------------------------------ Events ------------------------------- */

	/**
	* Event: Triggers when the actor has died (Health <= 0).
	* @param Owner : The owning actor that has died. 
	* @since 16/01/2024
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Health")
	void Event_OnDeath(AActor* Owner);

	/**
	* Event: Triggers when the actor has successfully had its health value changed.
	* @param NewHealth : The health value that the actor now has.
	* @since 16/01/2024
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Health")
	void Event_OnHealthChanged(float NewHealth);
};

/* ------------------------------------------------------------------------------- */
