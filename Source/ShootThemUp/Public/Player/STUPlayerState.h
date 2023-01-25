// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    void SetTeamID(uint32 ID)
    {
        TeamID = ID;
    }

    int32 GetTeamID()
    {
        return TeamID;
    }

    void SetTeamColor(FLinearColor Color)
    {
        TeamColor = Color;
    }

    FLinearColor GetTeamColor()
    {
        return TeamColor;
    }
    
private:
    
    int32 TeamID;
    FLinearColor TeamColor;
};
