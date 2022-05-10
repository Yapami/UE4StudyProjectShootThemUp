// Slava Ukraine

#include "Components/STUWeaponComponent.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Player/STUBaseCharacter.h"
#include "Weapon/STUBaseWeapon.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You
    // can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

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

    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
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
    auto Character = Cast<ACharacter>(GetOwner());
    if (CurrentWeapon)
    {
        FireStop();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[CurrentWeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    PlayAnimMontage(EquipAnimMontage);
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
    const auto NotifyEvents = EquipAnimMontage->Notifies;
    for (auto NotifyEvent : NotifyEvents)
    {
        const auto Notify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);
        if (Notify)
        {
            Notify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            return;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    const auto Owner = Cast<ACharacter>(GetOwner());
    if (Owner && Owner->GetMesh() == MeshComponent)
        EquipAnimIsGoing = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimIsGoing;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimIsGoing;
}

void USTUWeaponComponent::NextWeapon()
{
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    if (CanEquip())
    {
        EquipWeapon(CurrentWeaponIndex);
    }
}

void USTUWeaponComponent::FireStart()
{
    if (CanFire())
        CurrentWeapon->FireStart();
}

void USTUWeaponComponent::FireStop()
{
    ensure(CurrentWeapon);
    CurrentWeapon->FireStop();
}
