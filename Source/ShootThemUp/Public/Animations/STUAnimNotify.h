// Slava Ukraine

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "STUAnimNotify.generated.h"

/**
 *
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignture, USkeletalMeshComponent* MeshComponent);
UCLASS() class SHOOTTHEMUP_API USTUAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    FOnNotifiedSignture OnNotified;
};
