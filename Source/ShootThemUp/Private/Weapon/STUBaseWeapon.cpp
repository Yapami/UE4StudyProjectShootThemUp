// Slava Ukraine

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
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

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
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
        ChangeClip();
    }
    LogAmmo();
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return IsClipEmpty() && AmmoCurrentData.Clips == 0 && !AmmoCurrentData.Infinity;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return AmmoCurrentData.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!AmmoCurrentData.Infinity)
        --AmmoCurrentData.Clips;
    AmmoCurrentData.Bullets = AmmoDefaultData.Bullets;
    UE_LOG(LogTemp, Error,
           TEXT("-------------ChangeClip-----------"));
}

void ASTUBaseWeapon::LogAmmo()
{
    UE_LOG(LogTemp, Error, TEXT("Ammo: Bullets: %i, Clips: %i"), AmmoCurrentData.Bullets, AmmoCurrentData.Clips);
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
    const auto Controller = GetPlayerConrtoller();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
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
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                         ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::FireStart()
{
}

void ASTUBaseWeapon::FireStop()
{
}
