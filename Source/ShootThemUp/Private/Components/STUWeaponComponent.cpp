// Slava Ukraine

#include "Components/STUWeaponComponent.h"
#include "Animations/AnimUtils.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedNotify.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(WEAPON_COMPONENT, All, All);

const uint8 WeaponAmount = 2;
// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}

bool USTUWeaponComponent::AddAmmoToAllWeapons(int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        Weapon->TryToAddAmmo(ClipsAmount);
    }
    return true;
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponAmount, TEXT("Our character can have 2 weapon items only"))

        CurrentWeaponIndex = 0;
    SpawnWeapon();
    EquipWeapon(CurrentWeaponIndex);
    InitAnimations();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeaponIndex = 0;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapon()
{
    ensure(GetWorld());
    const auto Owner = Cast<ACharacter>(GetOwner());

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        ensure(Weapon);
        Weapon->SetOwner(Owner);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Weapon, Owner->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon,
                                               USceneComponent* CharacterMesh, FName& SocketName)
{
    if (!Weapon || !CharacterMesh)
        return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(CharacterMesh, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int8 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
        return;
    auto Character = Cast<ACharacter>(GetOwner());
    if (CurrentWeapon)
    {
        FireStop();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    PlayAnimMontage(EquipAnimMontage);
    const auto CurrentWeaponData = WeaponData.FindByPredicate(
        [&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });

    CurrentReloaadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    auto Character = Cast<ACharacter>(GetOwner());
    ensure(Character);
    EquipAnimIsGoing = true;
    Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify =
        AnimUtils::FindFirsNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindFirsNotifyByClass<USTUReloadFinishedNotify>(
            OneWeaponData.ReloadAnimMontage);

        if (!ReloadFinishedNotify)
            checkNoEntry();

        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    const auto Owner = Cast<ACharacter>(GetOwner());
    if (Owner && Owner->GetMesh() == MeshComponent)
        EquipAnimIsGoing = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    const auto Owner = Cast<ACharacter>(GetOwner());
    if (Owner && Owner->GetMesh() == MeshComponent)
        ReloadAnimIsGoing = false;
}

bool USTUWeaponComponent::CanFire() const
{
    UE_LOG(WEAPON_COMPONENT, Warning,
           TEXT("LOG5: USTUWeaponComponent::CanFire(), CurrentWeapon: %i, CanEquip(): %i"),
           CurrentWeapon, CanEquip());

    return CurrentWeapon && !EquipAnimIsGoing && !ReloadAnimIsGoing;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimIsGoing && !ReloadAnimIsGoing;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimIsGoing && !ReloadAnimIsGoing && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if (CurrentWeapon==AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for (const auto Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (CanReload())
    {
        CurrentWeapon->FireStop();
        CurrentWeapon->ChangeClip();
        PlayAnimMontage(CurrentReloaadAnimMontage);
        ReloadAnimIsGoing = true;
        EquipAnimIsGoing = false;
    }
}

void USTUWeaponComponent::NextWeapon()
{
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    if (CanEquip())
    {
        EquipWeapon(CurrentWeaponIndex);
    }
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

void USTUWeaponComponent::FireStart()
{
    UE_LOG(WEAPON_COMPONENT, Error, TEXT("LOG 1: USTUWeaponComponent::FireStart()"))
    if (CanFire())
        CurrentWeapon->FireStart();
}

void USTUWeaponComponent::FireStop()
{
    ensure(CurrentWeapon);
    CurrentWeapon->FireStop();
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData)
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}
