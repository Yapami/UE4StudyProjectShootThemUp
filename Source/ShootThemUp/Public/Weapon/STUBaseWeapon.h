// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();
    virtual void FireStart();
    virtual void FireStop();

    FOnClipEmptySignature OnClipEmpty;
    void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetWeaponUIData() const
    {
        return UIData;
    }

    FAmmoData GetAmmoCurrentData()
    {
        return AmmoCurrentData;
    }

    bool TryToAddAmmo(int32 ClipsAmount);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    APlayerController* GetPlayerConrtoller() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    FVector GetMuzzleWorldLocation();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzlseSocketName = "MuzzleSocket";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 15000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 20.0f;

    virtual void MakeShot();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData AmmoDefaultData = {15, 5, false};

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;

    UNiagaraComponent* SpawnMuzzleFX();

private:
    FAmmoData AmmoCurrentData;
    bool IsAmmoFull();
};
