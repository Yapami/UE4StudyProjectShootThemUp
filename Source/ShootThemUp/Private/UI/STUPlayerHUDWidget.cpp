// Slava Ukraine

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Weapon/STUBaseWeapon.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.f;

    return STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player)->GetHelthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Player);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

FString USTUPlayerHUDWidget::GetAmmunitionData() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return "";
    }

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Player);
    if (!WeaponComponent)
    {
        return "";
    }

    const auto CurrentWeapon = WeaponComponent->GetCurrentWeapon();
    check(CurrentWeapon);

    const auto AmmoCurrentData = CurrentWeapon->GetAmmoCurrentData();

    FString InfinitySymbol{TEXT("\u221E")};
    return FString::FromInt(AmmoCurrentData.Bullets) + " / " +
           (AmmoCurrentData.Infinity ? InfinitySymbol : FString::FromInt(AmmoCurrentData.Clips));
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return false;
    }
    auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player);
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return false;
    }
    auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player);
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }

    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}
