// ©2024 JDSherbert. All Rights Reserved.

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include <Runtime/Engine/Classes/Components/ActorComponent.h>
#include <Runtime/Engine/Classes/GameFramework/DamageType.h>

#include "HealthLog.h"

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
* Damage is assigned via floats.
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
    void InflictDamage(const float Amount);

    /**
	* Apply healing to this entity.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    void Heal(const float Amount);

    /**
	* Get the logs from health changes.
	* @since 16/01/2024
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintPure, Category = "Sherbert|Component|Health")
    FORCEINLINE const TArray<FDamageLog>& GetHealthLogs() const { return HealthLogs; }

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
	* Comparison method. Returns if the entity is dead or not (CurrentHealth > 0.0f).
	* @return bool : True if dead.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintPure, Category = "Sherbert|Component|Health")
    FORCEINLINE bool IsDead() const { return CurrentHealth > 0.0f ? false : true; }

    /**
	* Setter method. Sets current health. 
    * Preferred function for initialization and validation. Will not allow the set value to exceed MaxHealth or be below 0.0f.
	* @param NewCurrentHealth : Sets the CurrentHealth on this component.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    FORCEINLINE void SetCurrentHealth(float NewCurrentHealth) 
	{ 
		float TempHealth = CurrentHealth;
    	CurrentHealth = Clamp_CurrentHealth(NewCurrentHealth);

    	// Health changed?
    	if(TempHealth != CurrentHealth)
    	{
        	Event_OnHealthChanged(CurrentHealth);
        	DeathCheck();
    	}	
	}

    /**
	* Setter method. Sets max health. 
    * Preferred function for initialization and validation. Will not allow the set value to be below 0.0f.
    * Will also reduce CurrentHealth to match this value, if the new value is less than the CurrentHealth value.
	* @param NewMaxHealth : Sets the MaxHealth on this component, and the CurrentHealth if it overflows the max.
	* @since 16/01/2024
	* @author JDSherbert
	*/
    UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Health")
    FORCEINLINE void SetMaxHealth(float NewMaxHealth) 
	{ 
		float TempHealth = MaxHealth;
    	MaxHealth = Clamp_MaxHealth(NewMaxHealth);
		CurrentHealth = Clamp_CurrentHealth(CurrentHealth);

    	// Health changed?
    	if(TempHealth != CurrentHealth)
    	{
        	Event_OnHealthChanged(CurrentHealth);
        	DeathCheck();
    	}	
	}

private:

	UPROPERTY(Transient, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    TArray<FHealthLog> HealthLogs;

    /* Current health of the entity. */
    UPROPERTY(Transient, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    /* Maximum health of the entity. */
    UPROPERTY(EditDefaultsOnly, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    float MaxHealth;

	/* Current death state of the entity. True if dead. */
    UPROPERTY(Transient, Category = "Sherbert|Component|Health", meta = (AllowPrivateAccess = "true"))
    bool bDead;

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

	/**
	* Called internally. Polls IsDead() getter and fires the event if true.
	* Sets bDead variable and fires the death event ONLY ONCE upon death, even if additional triggers are recieved.
	* @since 16/01/2024
	* @author JDSherbert
	*/
	void DeathCheck() 
	{ 
		if(IsDead()) 
		{
			if(!bDead)
			{
				bDead = true;
				Event_OnDeath(this->GetOwner());
			}
		}
	}

	/**
	* Called internally. Files a log locally which can be read later in the session (via GetHealthLogs).
	* @since 16/01/2024
	* @author JDSherbert
	*/
	void LogHealth(const float Amount)
	{
		FHealthLog Entry;
    	Entry.Source = GetOwner()->GetFName();
    	Entry.Value = Amount;
    	Entry.Timestamp = FDateTime::Now().ToString();

    	HealthLogs.Add(Entry);
	}

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
 	* You'll probably want to hook your healthbar UI to this event.
	* @param NewHealth : The health value that the actor now has.
	* @since 16/01/2024
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Health")
	void Event_OnHealthChanged(float NewHealth);
};

/* ------------------------------------------------------------------------------- */
