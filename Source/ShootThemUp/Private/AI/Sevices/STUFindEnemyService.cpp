// Slava Ukraine


#include "AI/Sevices/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "FindEnemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
    float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if(Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
        if(PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
}
