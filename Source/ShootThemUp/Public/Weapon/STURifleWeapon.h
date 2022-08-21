// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:
    ASTURifleWeapon();

    virtual void FireStart() override;
    virtual void FireStop() override;

protected:
    virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ShootFrequency = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float HalfDegrees = 1.5f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    void MakeDamage(FHitResult& HitResult);

private:
    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    FTimerHandle FireTimer;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visibility);
};
