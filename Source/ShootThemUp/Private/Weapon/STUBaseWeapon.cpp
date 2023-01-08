// Slava Ukraine

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/STUBaseCharacter.h"

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (AmmoCurrentData.Infinity || IsAmmoFull() || ClipsAmount <= 0)
        return false;
    if (IsAmmoEmpty())
    {
        AmmoCurrentData.Clips =
            FMath::Clamp(AmmoCurrentData.Clips + ClipsAmount, 0, AmmoDefaultData.Clips + 1);
        OnClipEmpty.Broadcast(this);
        return true;
    }
    else if (AmmoCurrentData.Clips < AmmoDefaultData.Clips)
    {
        const auto NextClipsAmount = AmmoCurrentData.Clips + ClipsAmount;
        if (AmmoDefaultData.Clips - NextClipsAmount >= 0)
        {
            AmmoCurrentData.Clips = NextClipsAmount;
        }
        else
        {
            AmmoCurrentData.Clips = AmmoDefaultData.Clips;
            AmmoCurrentData.Bullets = AmmoDefaultData.Bullets;
        }
        return true;
    }
    else
    {
        AmmoCurrentData.Bullets = AmmoDefaultData.Bullets;
        return true;
    }
    return false;
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
    checkf(AmmoDefaultData.Bullets > 0, TEXT("Bullets count couldn`t be less or equal zero"));
    checkf(AmmoDefaultData.Clips > 0, TEXT("Clips count couldn`t be less or equal zero"));
    AmmoCurrentData = AmmoDefaultData;
}

void ASTUBaseWeapon::MakeShot()
{
    ensure(GetWorld());

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    --AmmoCurrentData.Bullets;
    if (!IsAmmoEmpty() && IsClipEmpty())
    {
        FireStop();
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return IsClipEmpty() && AmmoCurrentData.Clips == 0 && !AmmoCurrentData.Infinity;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return AmmoCurrentData.Bullets == 0;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(
        MuzzleFX, WeaponMesh, MuzzlseSocketName, FVector::ZeroVector, FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget, true);
}

bool ASTUBaseWeapon::IsAmmoFull()
{
    return AmmoCurrentData.Bullets == AmmoDefaultData.Bullets &&
           AmmoCurrentData.Clips == AmmoDefaultData.Clips;
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!AmmoCurrentData.Infinity)
    {
        if (AmmoCurrentData.Clips <= 0)
        {
            return;
        }
        --AmmoCurrentData.Clips;
    }

    AmmoCurrentData.Bullets = AmmoDefaultData.Bullets;
}

bool ASTUBaseWeapon::CanReload() const
{
    return AmmoCurrentData.Bullets < AmmoDefaultData.Bullets && AmmoCurrentData.Clips > 0;
}

APlayerController* ASTUBaseWeapon::GetPlayerConrtoller() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    ensure(Player);
    APlayerController* Controller = Player->GetController<APlayerController>();
    ensure(Controller);
    return Controller;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if(!STUCharacter)
    {
        return false;
    }

    if(STUCharacter->IsPlayerControlled())
    {
        const auto Controller = GetPlayerConrtoller();
        if (!Controller)
            return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = const_cast<ASTUBaseWeapon*>(this)->GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzlseSocketName);
    }
    
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation()
{
    return WeaponMesh->GetSocketLocation(MuzzlseSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                         ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::FireStart()
{
}

void ASTUBaseWeapon::FireStop()
{
}
