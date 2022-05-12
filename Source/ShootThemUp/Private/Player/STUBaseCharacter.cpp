// Slava Ukraine

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(HUI, All, All)

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(
          CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance
    // if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    if (!SpringArmComponent)
    {
        return;
    }
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    if (!CameraComponent)
    {
        return;
    }
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComrponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComrponent");
    ensure(WeaponComponent);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHelth());
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHelthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnObjectLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::AddControllerYawInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::SetIsRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::SetIsRunning);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::FireStart);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent,
                                     &USTUWeaponComponent::FireStop);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::Reload);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
        return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween =
        FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::SetIsRunning()
{
    WantsToRun = !WantsToRun;
}

void ASTUBaseCharacter::OnDeath()
{
    WeaponComponent->FireStop();
    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnObjectLanded(const FHitResult& Hit)
{
    const float ZVelocity = FMath::Abs(GetVelocity().Z);

    if (ZVelocity < DistanceRange.X)
        return;

    const float Damage = FMath::GetMappedRangeValueClamped(DistanceRange, DamageRange, ZVelocity);
    TakeDamage(Damage, FDamageEvent(), nullptr, nullptr);
}
