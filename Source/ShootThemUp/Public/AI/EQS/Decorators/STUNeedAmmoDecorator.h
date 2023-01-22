// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STUCoreTypes.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUNeedAmmoDecorator.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
    USTUNeedAmmoDecorator();
    
protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AI")
    TSubclassOf<ASTUBaseWeapon> WeaponType;
    
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
