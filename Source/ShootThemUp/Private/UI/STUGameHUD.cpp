// Slava Ukraine

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(GameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair()
{
    const FVector2D CenterScreen(Canvas->SizeX * 0.5, Canvas->SizeY * 0.5);
    DrawLine(CenterScreen.X - 10.f, CenterScreen.Y, CenterScreen.X + 10, CenterScreen.Y,
             FLinearColor::Green, 2.f);
    DrawLine(CenterScreen.X, CenterScreen.Y - 10, CenterScreen.X, CenterScreen.Y + 10,
             FLinearColor::Green, 2.f);
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!PlayerHUDWidgetClass)
    {
        UE_LOG(GameHUD, Warning, TEXT("LOG 10: PlayerHUDWidgetClass: %i"), PlayerHUDWidgetClass);
        return;
    }
    
    const auto HUDWidget = CreateWidget(GetWorld(), PlayerHUDWidgetClass);

    if (!HUDWidget)
    {
        return;
    }

    HUDWidget->AddToViewport();
}
