// Slava Ukraine

#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
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

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);

    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
