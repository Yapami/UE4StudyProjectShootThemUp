// Slava Ukraine

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

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