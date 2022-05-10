// Slava Ukraine


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void ASTURifleWeapon::FireStart()
{
    MakeShot();
    GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ASTURifleWeapon::MakeShot,
                                           ShootFrequency, true);
}

void ASTURifleWeapon::FireStop()
{
    GetWorld()->GetTimerManager().ClearTimer(FireTimer);
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
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 4.f);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red,
                      false, 3.0f, 0, 3.0f);

        MakeDamage(HitResult);
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
    if (HitResult.GetActor()   )
    {
        HitResult.GetActor()->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerConrtoller(),
                                         GetOwner());
    }
}
