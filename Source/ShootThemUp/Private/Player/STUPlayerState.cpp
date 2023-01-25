// Slava Ukraine


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kill: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
