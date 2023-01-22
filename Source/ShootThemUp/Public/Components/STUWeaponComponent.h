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
    virtual void FireStart();
    void FireStop();
    virtual void NextWeapon();
    void Reload();
    bool GetWeaponUIData(FWeaponUIData& UIData);
    ASTUBaseWeapon* GetCurrentWeapon()
    {
        return CurrentWeapon;
    }
    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

    bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

    bool AddAmmoToAllWeapons(int32 ClipsAmount);
     
protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    bool CanFire() const;
    bool CanEquip() const;

    void EquipWeapon(int8 WeaponIndex);

    int8 CurrentWeaponIndex = 0;

private:
    void SpawnWeapon();
    
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* CharacterMesh,
                              FName& SocketName);
    
    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    bool EquipAnimIsGoing = false;
    bool ReloadAnimIsGoing = false;
    
    bool CanReload() const;

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    UAnimMontage* CurrentReloaadAnimMontage = nullptr;
};
