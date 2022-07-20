// Slava Ukraine

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class USTUWeaponFXComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUProjectile();
    void SetShotDirection(const FVector& Direction);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere)
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditAnywhere)
    float DamageRadius = 500.f;

    UPROPERTY(EditAnywhere)
    float DamageAmount = 50.f;

    UPROPERTY(EditAnywhere)
    bool DoFullDamage = false;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    AController* GetController() const;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                         UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                         const FHitResult& Hit);
};
