// Slava Ukraine

#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

DEFINE_LOG_CATEGORY_STATIC(RIFLE_WEAPON_COMPONENT, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::FireStart()
{
    InitMuzzleFX();

    GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ASTURifleWeapon::MakeShot,
                                           ShootFrequency, true);
    MakeShot();
}

void ASTURifleWeapon::FireStop()
{
    SetMuzzleFXVisibility(false);

    GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::MakeShot()
{
    ensure(GetWorld());
    if (IsAmmoEmpty())
    {
        FireStop();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        FireStop();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 4.f);
        // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red,
        //              false, 3.0f, 0, 3.0f);

        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0,
                      3.0f);
    }
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    float HalfRad = FMath::DegreesToRadians(HalfDegrees);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
    if (HitResult.GetActor())
    {
        HitResult.GetActor()->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerConrtoller(),
                                         GetOwner());
    }
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visibility)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!Visibility);
        MuzzleFXComponent->SetVisibility(Visibility, true);
    }
}
