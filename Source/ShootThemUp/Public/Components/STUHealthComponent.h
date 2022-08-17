// Slava Ukraine

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUHealthComponent();

    float GetHelth() const
    {
        return Health;
    }

    FOnHelthChanged OnHealthChanged;

    UFUNCTION(BlueprintCallable)
    bool IsDead() const
    {
        return FMath::IsNearlyZero(Health);
    }

    FOnDeath OnDeath;

    UFUNCTION(BlueprintCallable, Category = "Heal")
    float GetHelthPercent() const
    {
        return Health / MaxHealth;
    }

    bool TryToAddHealth(float HelthAmount);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health",
              meta = (ClampMin = "0", ClampMax = "1000"))
    float MaxHealth = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = "0", ClampMax = "1000", EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = "0", ClampMax = "1000", EditCondition = "AutoHeal"))
    float HealDelay = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal",
              meta = (ClampMin = "0", EditCondition = "AutoHeal"))
    float HealModifier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

private:
    float Health = 0.f;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                         class AController* InstigatedBy, AActor* DamageCauser);

    FTimerHandle HealTimerHandle;

    void Healing();
    void SetHealth(float Health);

    void PlayCameraShake();
};
