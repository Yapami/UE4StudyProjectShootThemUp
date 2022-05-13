// Slava Ukraine

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (Player)
    {
        auto HealthComponent = Cast<USTUHealthComponent>(
            Player->GetComponentByClass(USTUHealthComponent::StaticClass()));
        return HealthComponent->GetHelthPercent();
    }
    else
    {
        return 0.f;
    }
}