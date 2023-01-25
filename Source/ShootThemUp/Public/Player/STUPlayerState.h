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

    void AddKill()
    {
        ++KillsNum;
    }
    
    int32 GetKillsNum() const
    {
        return KillsNum;
    }

    
    void AddDeath()
    {
        ++DeathsNum;
    }

    int32 GetDeathsNum() const
    {
        return DeathsNum;
    }

    void LogInfo();
private:
    
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
