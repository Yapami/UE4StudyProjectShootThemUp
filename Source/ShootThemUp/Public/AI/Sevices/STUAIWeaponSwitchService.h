// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUAIWeaponSwitchService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponSwitchService : public UBTService
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SwitchProbability = 0.5;
    
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
