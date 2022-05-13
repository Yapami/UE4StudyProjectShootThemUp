// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
    GENERATED_BODY()

    void DrawHUD() override;
    void DrawCrossHair();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    virtual void BeginPlay() override;
};
