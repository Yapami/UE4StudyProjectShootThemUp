// Slava Ukraine

#include "Weapon/STULauncherWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/STUProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(LauncherWeapon, All, All);

void ASTULauncherWeapon::FireStart()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
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

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile =
        GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    UE_LOG(LauncherWeapon, Error, TEXT("LOG 1: "));
    
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
    SpawnMuzzleFX();
}
