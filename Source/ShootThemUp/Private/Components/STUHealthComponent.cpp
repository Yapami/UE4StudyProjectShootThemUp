// Slava Ukraine

#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(AAAAAAAAAAAAAAHELTH_INFO, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* const Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage,
                                          const UDamageType* DamageType, AController* InstigatedBy,
                                          AActor* DamageCauser)
{
    if (Damage <= 0.f || IsDead())
        return;

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::Healing,
                                               HealUpdateTime, true, HealDelay);  
    }
}

void USTUHealthComponent::Healing()
{
    SetHealth(Health + HealModifier);
    if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHelthChanged.Broadcast(Health);
}

bool USTUHealthComponent::TryToAddHealth(float HelthAmount)
{
    if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        return false;
    }
    else
    {
        SetHealth(Health + HelthAmount);
        return true;
    }
}
