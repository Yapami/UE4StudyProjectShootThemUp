// Slava Ukraine

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Styling/SlateBrush.h"
#include "STUPlayerHUDWidget.generated.h"
/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetAmmunitionData() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;
};
