// Slava Ukraine


#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"


USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location"; //Visual name of this node 
}

//Here we compute random point on the map and write this point to the blackboard
EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if(!Controller || !Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    const auto Pawn = Controller->GetPawn();
    if(!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    const auto NavigationSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if(!NavigationSystem)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavLocation;
    
    const auto Found = NavigationSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation);
    if(!Found)
    {
        return  EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
