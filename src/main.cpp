#include "raylib.h"
#include "screens.h"

GameScreen currentScreen = LOGO;
Font font = { 0 };
Music music = { 0 };
Sound fxChange = { 0 };
Sound fxMove = { 0 };
Sound fxStageMove = { 0 };
Sound fxStageCorrect = { 0 };
Sound fxStageWrong = { 0 };

const int screenWidth = 800;
const int screenHeight = 450;


float transAlpha = 0.0f;
bool onTransition = false;
bool transFadeOut = false;
int transFromScreen = -1;
GameScreen transToScreen = UNKNOWN;

void ChangeToScreen(GameScreen);

void TransitionToScreen(GameScreen);
void UpdateTransition();
void DrawTransition();
void UpdateDrawFrame();

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Pikachu");
    DisableCursor();
    SetWindowIcon(LoadImage("resources/icon.png"));
    MaximizeWindow();
    InitAudioDevice();

    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/track.mp3");
    fxChange = LoadSound("resources/African4.mp3");
    fxMove = LoadSound("resources/African2.mp3");
    fxStageMove = LoadSound("resources/Retro1.mp3");
    fxStageCorrect = LoadSound("resources/BigEggcollection1.mp3");
    fxStageWrong = LoadSound("resources/RetroEventWrongSimple03.mp3");
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    currentScreen = LOGO;
    InitLogoScreen();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxChange);
    UnloadSound(fxMove);
    UnloadSound(fxStageCorrect);
    UnloadSound(fxStageMove);
    UnloadSound(fxStageWrong);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void ChangeToScreen(GameScreen screen)
{
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }

    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

void UpdateTransition()
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }

            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;
            transFadeOut = true;
        }
    }
    else
    {
        transAlpha -= 0.02f;
        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

void DrawTransition()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}


void UpdateDrawFrame()
{
    UpdateMusicStream(music);

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen() == 1) TransitionToScreen(TITLE);
            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
                else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);
                else if (FinishTitleScreen() == 3) TransitionToScreen(ENDING);
            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen() == 1) TransitionToScreen(TITLE);
            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);
            } break;
            default: break;
        }
    }
    else UpdateTransition();

    BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }
    
        if (onTransition) DrawTransition();
    EndDrawing();
}
