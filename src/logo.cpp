#include "raylib.h"
#include "screens.h"

int framesCounter = 0;
int finishLogo = 0;
int lettersCount = 0;
int titleposx = 0;
int titleposy = 0;
float alpha = 1.0f;

void InitLogoScreen(){
    titleposx = GetScreenWidth()/2 + 25 - MeasureTextEx(font, "Pikachu", 150 , 0).x;
    titleposy = GetScreenHeight()/2 + 25 - MeasureTextEx(font, "Pikachu", 150 , 0).y;
}

void UpdateLogoScreen(){

    framesCounter++;

    if (lettersCount < 10){
        if (framesCounter/12){
            lettersCount++;
            framesCounter = 0;
        }
    }
    else{
        if (IsKeyPressed(KEY_ENTER)){
            finishLogo = 1;
            PlaySound(fxChange);
        }
    }
}

void DrawLogoScreen(){
    DrawText(TextSubtext("Pikachu", 0, lettersCount), titleposx, titleposy, 150, Fade(BLACK, alpha));
    if (framesCounter > 20) DrawText("Press ENTER to continue", titleposx + 126, titleposy + 150, 25, Fade(DARKGRAY, alpha));
}

void UnloadLogoScreen(){
    
}

int FinishLogoScreen(){
    return finishLogo;
}
