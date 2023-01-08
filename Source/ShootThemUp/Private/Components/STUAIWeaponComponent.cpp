// Slava Ukraine


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::FireStart()
{
    if(!CanFire())
    {
        return;
    }

    if(CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    {
        CurrentWeapon->FireStart();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if(!CanEquip())
    {
        return;
    }

    uint8 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if(!Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if(CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
