// Slava Ukraine


#include "AI/Sevices/STUAIWeaponSwitchService.h"

#include "AIController.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"

void USTUAIWeaponSwitchService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
    float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(Controller)
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
        if(WeaponComponent && SwitchProbability > 0 && SwitchProbability >= FMath::FRand())
        {
            WeaponComponent->NextWeapon();
        }
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
