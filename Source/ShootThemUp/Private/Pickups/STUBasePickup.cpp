// Slava Ukraine

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }

    UE_LOG(LogTemp, Warning, TEXT("LOG 1: Collision Overlap"));
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetRootComponent()->SetVisibility(false, true);

    FTimerHandle Timer;
    GetWorld()->GetTimerManager().SetTimer(Timer, this, &ASTUBasePickup::Respawn, RespawnTime,
                                           false);
}

void ASTUBasePickup::Respawn()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetRootComponent()->SetVisibility(true, true);
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
