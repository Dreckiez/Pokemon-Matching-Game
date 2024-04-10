#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } GameScreen;

extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxChange;
extern Sound fxMove;
extern Sound fxStageMove;
extern Sound fxStageCorrect;
extern Sound fxStageWrong;

#ifdef __cplusplus
extern "C" {
#endif

void InitLogoScreen();
void UpdateLogoScreen();
void DrawLogoScreen();
void UnloadLogoScreen();
int FinishLogoScreen();

void InitTitleScreen();
void UpdateTitleScreen();
void DrawTitleScreen();
void UnloadTitleScreen();
int FinishTitleScreen();

void InitOptionsScreen();
void UpdateOptionsScreen();
void DrawOptionsScreen();
void UnloadOptionsScreen();
int FinishOptionsScreen();

void InitGameplayScreen();
void UpdateGameplayScreen();
void DrawGameplayScreen();
void UnloadGameplayScreen();
int FinishGameplayScreen();

void InitEndingScreen();
void UpdateEndingScreen();
void DrawEndingScreen();
void UnloadEndingScreen();
int FinishEndingScreen();

#ifdef __cplusplus
}
#endif

#endif