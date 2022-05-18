// Slava Ukraine

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<FWeaponData> WeaponData;
    void FireStart();
    void FireStop();
    void NextWeapon();
    void Reload();
    bool GetWeaponUIData(FWeaponUIData& UIData);
    ASTUBaseWeapon* GetCurrentWeapon()
    {
        return CurrentWeapon;
    }
     
protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditAnywhere, Category = "Weapon")
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

private:
    void SpawnWeapon();

    TArray<ASTUBaseWeapon*> Weapons;
    int8 CurrentWeaponIndex = 0;
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* CharacterMesh,
                              FName& SocketName);

    void EquipWeapon(int8 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    bool EquipAnimIsGoing = false;
    bool ReloadAnimIsGoing = false;

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();

    UAnimMontage* CurrentReloaadAnimMontage = nullptr;
};
