#include "raylib.h"
#include "screens.h"
#include<time.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

int curposx = 0, curposy = 0; // Current position
int finishGame = 0; // Check if game is finished, 0 = NO, 1 = YES
char** stage = 0;
int cell1x = -1, cell1y = -1; //Positon of 1st selected cell
int cell2x = -2, cell2y = -2; //Position of 2nd selected cell
int selected = 0; // Number of selected cell
int cellsize = 60; //Define size of a cell
int StageCenterWidth = 0, StageCenterHeight = 0; //Help centering the stage
int textCenterWidth = 0, textCenterHeight = 0; //Help centering the character inside each cell
int Highscore = 0; //Score
int checkstate = 0; //0 = there's no checking, 1 = Matchable, 2 = Not matchable
int framescounter = 0;
int midp1x = -2, midp1y = -2, midp2x = -2, midp2y = -2; //Positon of midpoints for drawing matching line
int help1x = -1, help1y = -1, help2x = -2, help2y = -2; //Position of Suggested cells
int helpstate = 0; //Check if there are possible cells to suggest, 0 = not checking or not found, 1 = found  
int drawline = 0; //Determine what kind of matchable line to draw, 0 = no line, 1 = I line, 2 = L line, 3 = Z || U line
int timercounter = 0, timerlifetime = 36000;
int shufflestate = 0; // 0 = no shuffle, 1 = wait screen to shuffle

//Generate stage && set initial value for other variables
void InitGameplayScreen(){
    StageCenterWidth = GetScreenWidth()/2 - 8 * cellsize;  
    StageCenterHeight = GetScreenHeight()/2 - 4 * cellsize;
    textCenterWidth = StageCenterWidth + 24;
    textCenterHeight = StageCenterHeight + 23;
    //Allocate and set all the character to space (' ')
    srand(time(0));
    stage = new char*[8];
    for (int i = 0; i < 8; i++) stage[i] = new char[16];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 16; j++) stage[i][j] = ' ';
    }
    //Generate character in pairs
    int pairsnum = (8*16)/2;
    while(pairsnum){
        int col1 = rand() % 8;
        int row1 = rand() % 16;
        int col2 = rand() % 8;
        int row2 = rand() % 16;
        if (stage[col1][row1] == ' ' && stage[col2][row2] == ' ' && col1 != col2 && row1 != row2){
            stage[col1][row1] = stage[col2][row2] = (char) (rand() % 26 + 65);
            pairsnum--;
        }
    }
}

bool EndGame(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 16; j++){
            if (stage[i][j] != ' ') return 0;
        }
    }
    return 1;
}

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

bool CheckLineX(int cell1x, int cell2x, int y){
    for (int i = cell1x + 1; i < cell2x; i++)
        if (stage[y][i] != ' ') return 0;
    return 1;
}

bool CheckLineY(int cell1y, int cell2y, int x){
    for (int i = cell1y + 1; i < cell2y; i++)
        if (stage[i][x] != ' ') return 0;
    return 1;
}

bool Icheck(int cell1x, int cell2x, int cell1y, int cell2y){
    if (cell1y == cell2y){
        if (cell1x > cell2x) swap(cell1x, cell2x);
        if (CheckLineX(cell1x, cell2x, cell1y)){
            return 1;
        }
        else return 0;
    }
    if (cell1x == cell2x){
        if (cell1y > cell2y) swap(cell1y, cell2y);
        if (CheckLineY(cell1y, cell2y, cell1x)){
            return 1;
        }
        else return 0;
    }
    return 0;
}

bool Lcheck(int cell1x, int cell2x, int cell1y, int cell2y, int type){
    int tempx, tempy;
    if (cell1x > cell2x){
        swap(cell1x, cell2x);
        swap(cell1y, cell2y);
    }
    tempx = cell2x;
    tempy = cell1y;
    if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(tempx, cell2x, tempy, cell2y) && stage[tempy][tempx] == ' '){
        if (type == 1){
            midp1x = tempx; 
            midp1y = tempy;
        }
        else {
            midp2x = tempx;
            midp2y = tempy; 
        }
        return 1;
    }
    tempx = cell1x;
    tempy = cell2y;
    if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(tempx, cell2x, tempy, cell2y) && stage[tempy][tempx] == ' '){
        if (type == 1){
            midp1x = tempx; 
            midp1y = tempy;
        }
        else {
            midp2x = tempx;
            midp2y = tempy; 
        }
        return 1;
    }
    return 0;
}

bool Zcheck(int cell1x, int cell2x, int cell1y, int cell2y){
    int tempx, tempy;
    if (cell1x > cell2x){
        swap(cell1x, cell2x);
        swap(cell1y, cell2y);
    }
    // C1---
    //     |
    //     ---C2   check
    tempx = cell2x - 1;
    tempy = cell2y;
    while(tempx > cell1x){
        if (Lcheck(cell1x, tempx, cell1y, tempy, 1) && Icheck(cell2x, tempx, cell2y, tempy) && Icheck(tempx, cell2x, tempy, cell2y) && stage[cell1y][tempx] == ' '  && stage[cell2y][tempx] == ' '){
            midp2x = tempx; 
            midp2y = tempy;
            return 1;
        }
        tempx--;
    }
    // C1
    //  |
    //  ------
    //       |
    //      C2  check
    tempx = cell1x;
    tempy = cell1y + 1;
    while(tempy < cell2y) {
        if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(tempx, cell2x, tempy, cell2y, 2) && stage[tempy][tempx] == ' ' && stage[tempy][cell2x] == ' '){
            midp1x = tempx; 
            midp1y = tempy;
            return 1;
        }
        tempy++;
    }
    //        C2
    //        |
    //  ------
    // |    
    // C1      check
    tempx = cell1x;
    tempy = cell1y - 1;
    while(tempy > cell2y) {
        if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(tempx, cell2x, tempy, cell2y, 2) && stage[tempy][tempx] == ' ' && stage[tempy][cell2x] == ' '){
            midp1x = tempx; 
            midp1y = tempy;
            return 1;
        }
        tempy--;
    }
    return 0;
}

bool Ucheck(int cell1x, int cell2x, int cell1y, int cell2y){
    if (cell1x > cell2x){
        swap(cell1x, cell2x);
        swap(cell1y, cell2y);
    }
    int tempx, tempy;
    //Check up
    tempx = cell1x;
    for (tempy = cell1y - 1; tempy >= -1; tempy--){
        if (tempy == -1){
            if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(cell2x, cell2x, cell2y, tempy)){
                midp1x = tempx; 
                midp1y = tempy;
                midp2x = cell2x;
                midp2y = tempy;
                return 1;
            }
        }
        else {
            if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(tempx, cell2x, tempy, cell2y, 2) && stage[tempy][tempx] == ' '){
                midp1x = tempx; 
                midp1y = tempy;
                return 1;
            }
        }
    }

    //Check down
    tempx = cell1x;
    for (tempy = cell1y + 1; tempy <= 8; tempy++){
        if (tempy == 8){
            if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(cell2x, cell2x, cell2y, tempy)){
                midp1x = tempx; 
                midp1y = tempy;
                midp2x = cell2x;
                midp2y = tempy;
                return 1;
            }
        }
        else {
            if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(tempx, cell2x, tempy, cell2y, 2) && stage[tempy][tempx] == ' '){
                midp1x = tempx; 
                midp1y = tempy;
                return 1;
            }
        }
    }

    //Check right
    tempy = cell1y;
    for (tempx = cell1x + 1; tempx <= 16; tempx++){
        if (tempx == 16){
            if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(cell2x, tempx, cell2y, cell2y)){
                midp1x = tempx;
                midp1y = tempy;
                midp2x = tempx;
                midp2y = cell2y;
                return 1;
            }
        }
        else {
            if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(cell2x, tempx, cell2y, tempy, 2) && stage[tempy][tempx] == ' '){
                midp1x = tempx; 
                midp1y = tempy;
                return 1;
            }
        }
    }

    //Check left
    tempy = cell1y;
    for (tempx = cell1x - 1; tempx >= -1; tempx--){
        if (tempx == -1){
            if (Icheck(cell1x, tempx, cell1y, tempy) && Icheck(cell2x, tempx, cell2y, cell2y)){
                midp1x = tempx; 
                midp1y = tempy;
                midp2x = tempx;
                midp2y = cell2y;
                return 1;
            }
        }
        else {
            if (Icheck(cell1x, tempx, cell1y, tempy) && Lcheck(tempx, cell2x, tempy, cell2y, 2) && stage[tempy][tempx] == ' '){
                midp1x = tempx; 
                midp1y = tempy;
                return 1;
            }
        }
    }

    return 0;
}

void DrawI(int cell1x, int cell1y, int cell2x, int cell2y){
    int startpx = cell1x * cellsize + textCenterWidth + 5, startpy = cell1y * cellsize + textCenterHeight + 5;
    int endpx = cell2x * cellsize + textCenterWidth + 5, endpy = cell2y * cellsize + textCenterHeight + 5;
    DrawLineEx({(float) startpx, (float)startpy}, {(float)endpx, (float)endpy}, 5, VIOLET);
}

void DrawL(int cell1x, int cell1y, int cell2x, int cell2y, int midp1x, int midp1y){
    int startpx = cell1x * cellsize + textCenterWidth + 5, startpy = cell1y * cellsize + textCenterHeight + 5;
    int endpx = cell2x * cellsize + textCenterWidth + 5, endpy = cell2y * cellsize + textCenterHeight + 5;
    DrawLineEx({(float) startpx, (float)startpy}, {(float) midp1x * cellsize + textCenterWidth + 5, (float)midp1y * cellsize + textCenterHeight + 5}, 5, VIOLET);
    DrawLineEx({(float) midp1x * cellsize + textCenterWidth + 5, (float)midp1y * cellsize + textCenterHeight + 5}, {(float)endpx, (float)endpy}, 5, VIOLET);
}

void DrawZandU(int cell1x, int cell1y, int cell2x, int cell2y, int midp1x, int midp1y, int midp2x, int midp2y){
    if (cell1x > cell2x){
        swap(cell1x, cell2x);
        swap(cell1y, cell2y);
    }
    int startpx = cell1x * cellsize + textCenterWidth + 5, startpy = cell1y * cellsize + textCenterHeight + 5;
    int endpx = cell2x * cellsize + textCenterWidth + 5, endpy = cell2y * cellsize + textCenterHeight + 5;
    DrawLineEx({(float) startpx, (float)startpy}, {(float) midp1x * cellsize + textCenterWidth + 5, (float)midp1y * cellsize + textCenterHeight + 5}, 5, VIOLET);
    DrawLineEx({(float) midp1x * cellsize + textCenterWidth + 5, (float)midp1y * cellsize + textCenterHeight + 5}, {(float) midp2x * cellsize + textCenterWidth + 5, (float)midp2y * cellsize + textCenterHeight + 5}, 5, VIOLET);
    DrawLineEx({(float) midp2x * cellsize + textCenterWidth + 5, (float)midp2y * cellsize + textCenterHeight + 5}, {(float)endpx, (float)endpy}, 5, VIOLET);
}

bool HelpCheck(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 16; j++){
            if (stage[i][j] != ' '){
                for (int a = 0; a < 8; a++){
                    for (int b = 0; b < 16; b++){
                        if (stage[a][b] == stage[i][j] && (a != i || b != j)){
                            if (Icheck(j, b, i, a) || Lcheck(j, b, i, a, 1) || Zcheck(j, b, i, a) || Ucheck(j, b, i, a)){
                                help1x = j; help1y = i;
                                help2x = b; help2y = a;
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void swap1()
{   
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 16; j++){
			if(stage[i][j] != ' '){
				for(int k = 0; k < 8; k++){
					for(int h = 0; h < 16; h++){
						if(stage[k][h] != ' '){
							swap(stage[i][j], stage[k][h]);
						}
					}
				}
			}
		}
	}
}

void Shuffle(){
    srand(time(0));
    while (HelpCheck() == 0){   
        int col1 = rand() % 8;
        int col2 = rand() % 8;
        int row1 = rand() % 16;
        int row2 = rand() % 16;
        while(stage[col1][row1] == ' '){
            col1 = rand() % 8;
            row1 = rand() % 16;
        }
        while((stage[col2][row2] == ' ' || (col1 == col2 && row1 == row2)) && stage[col1][row1] != stage[col2][row2]){
            col2 = rand() % 8;
            row2 = rand() % 16;
        }
        swap(stage[col1][row1], stage[col2][row2]);
    }
    
}


void UpdateGameplayScreen(){
    timercounter++; //Update timercounter, timercounter = 60 -> 1s has passed since the FPS is set at 60

    if (HelpCheck() == 0 && EndGame() == 0) shufflestate = 1;

    if (shufflestate != 0){
        if (framescounter > 60){
            Shuffle();
            shufflestate = 0;
            framescounter = 0;
        }
        framescounter++;
    }


    //Move 
    if (IsKeyPressed(KEY_UP)){
        PlaySound(fxStageMove);
        curposy--;
    }
    if (IsKeyPressed(KEY_DOWN)){
        PlaySound(fxStageMove);
        curposy++;
    }
    if (curposy < 0) curposy = 7; //Loopback
    else if (curposy > 7) curposy = 0;
    if (IsKeyPressed(KEY_LEFT)){
        PlaySound(fxStageMove);
        curposx--;
    }
    if (IsKeyPressed(KEY_RIGHT)){
        PlaySound(fxStageMove);
        curposx++;
    }
    if (curposx < 0) curposx = 15; //Loopback
    else if (curposx > 15) curposx = 0;

    //Select cell
    if (IsKeyPressed(KEY_ENTER)){
        if (stage[curposy][curposx] != ' '){
            if (selected == 0){
                PlaySound(fxChange);
                cell1x = curposx;
                cell1y = curposy;
                selected++;
            }
            else if (selected == 1){
                PlaySound(fxChange);
                cell2x = curposx;
                cell2y = curposy;
                selected++;
            }

            //Checking if matchable
            if (selected == 2 && checkstate == 0){
                //Wrong case
                if ((cell1x == cell2x && cell1y == cell2y) || stage[cell1y][cell1x] != stage[cell2y][cell2x]){
                    checkstate = 2;
                }
                else {
                    if (Icheck(cell1x, cell2x, cell1y, cell2y)){
                        drawline = 1;
                        checkstate = 1;   
                    }
                    else if (Lcheck(cell1x, cell2x, cell1y, cell2y, 1)){
                        drawline = 2;
                        checkstate = 1;
                    }
                    else if (Zcheck(cell1x, cell2x, cell1y, cell2y)){
                        drawline = 3;
                        checkstate = 1;
                    }
                    else if (Ucheck(cell1x, cell2x, cell1y, cell2y)){
                        drawline = 3;
                        checkstate = 1;
                    }
                    else{
                        checkstate = 2;
                    }
                }
                
            }
        }
            
    }

    //if Checked, delay to draw 
    if (checkstate != 0){
        if (framescounter > 60){
            if (checkstate == 1){
                stage[cell1y][cell1x] = stage[cell2y][cell2x] = ' ';
                Highscore += 20;
                drawline = 0;
                midp1x = midp1y = midp2x = midp2y = -2;
            }
            else if (checkstate == 2){
                Highscore -= 20;
            }
            cell1x = -1;
            cell1y = -1;
            cell2x = -2;
            cell2y = -2;
            selected = 0;
            framescounter = 0;
            checkstate = 0;
        }
        framescounter++;
    }

    if (IsKeyPressed(KEY_H)){
        PlaySound(fxStageCorrect);
        if (HelpCheck()) helpstate = 1;
    }

    if (helpstate != 0){
        framescounter++;
        if (framescounter > 60){
            helpstate = 0;
            Highscore -= 75;
            framescounter = 0;
            help1x = help1y = -1;
            help2x = help2y = -2;
        }
    }

    if (timercounter >= 60){
        timerlifetime -= timercounter;
        timercounter = 0;
    }
    
    if (EndGame()){
        finishGame = 1;
        PlaySound(fxChange);
    }
}

void DrawGameplayScreen(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 16; j++){
            
            if (curposx == j && curposy == i){
                DrawRectangle(j * cellsize + StageCenterWidth, i * cellsize + StageCenterHeight, cellsize, cellsize, GRAY);
                DrawText(TextFormat("%c", stage[i][j]), j * cellsize + textCenterWidth, i * cellsize + textCenterHeight, 20, RAYWHITE);
            }
            else {
                DrawRectangleLines(j * cellsize + StageCenterWidth, i * cellsize + StageCenterHeight, cellsize, cellsize, LIGHTGRAY);
                DrawText(TextFormat("%c", stage[i][j]), j * cellsize + textCenterWidth, i * cellsize + textCenterHeight, 20, BLACK);
            }

            if (selected > 0){
                if (cell1x > -1 && cell1y > -1 ){
                    DrawRectangle(cell1x * cellsize + StageCenterWidth, cell1y * cellsize + StageCenterHeight, cellsize, cellsize, BLUE);
                    DrawText(TextFormat("%c", stage[cell1y][cell1x]), cell1x * cellsize + textCenterWidth, cell1y * cellsize + textCenterHeight, 20, RAYWHITE);
                }
                if (cell2x > -1 && cell2y > -1 ){
                    DrawRectangle(cell2x * cellsize + StageCenterWidth, cell2y * cellsize + StageCenterHeight, cellsize, cellsize, BLUE);
                    DrawText(TextFormat("%c", stage[cell2y][cell2x]), cell2x * cellsize + textCenterWidth, cell2y * cellsize + textCenterHeight, 20, RAYWHITE);
                }
                if (checkstate == 1){
                    DrawRectangle(cell1x * cellsize + StageCenterWidth, cell1y * cellsize + StageCenterHeight, cellsize, cellsize, GREEN);
                    DrawText(TextFormat("%c", stage[cell1y][cell1x]), cell1x * cellsize + textCenterWidth, cell1y * cellsize + textCenterHeight, 20, RAYWHITE);
                    DrawRectangle(cell2x * cellsize + StageCenterWidth, cell2y * cellsize + StageCenterHeight, cellsize, cellsize, GREEN);
                    DrawText(TextFormat("%c", stage[cell2y][cell2x]), cell2x * cellsize + textCenterWidth, cell2y * cellsize + textCenterHeight, 20, RAYWHITE);
                    
                    if (drawline == 1) DrawI(cell1x, cell1y, cell2x, cell2y);
                    else if (drawline == 2) DrawL(cell1x, cell1y, cell2x, cell2y, midp1x, midp1y);
                    else if (drawline == 3) DrawZandU(cell1x, cell1y, cell2x, cell2y, midp1x, midp1y, midp2x, midp2y);
                }
                else if (checkstate == 2){
                    DrawRectangle(cell1x * cellsize + StageCenterWidth, cell1y * cellsize + StageCenterHeight, cellsize, cellsize, RED);
                    DrawText(TextFormat("%c", stage[cell1y][cell1x]), cell1x * cellsize + textCenterWidth, cell1y * cellsize + textCenterHeight, 20, RAYWHITE);
                    DrawRectangle(cell2x * cellsize + StageCenterWidth, cell2y * cellsize + StageCenterHeight, cellsize, cellsize, RED);
                    DrawText(TextFormat("%c", stage[cell2y][cell2x]), cell2x * cellsize + textCenterWidth, cell2y * cellsize + textCenterHeight, 20, RAYWHITE);
                }
                
            } 

            if (helpstate == 1){
                DrawRectangle(help1x * cellsize + StageCenterWidth, help1y * cellsize + StageCenterHeight, cellsize, cellsize, BLUE);
                DrawText(TextFormat("%c", stage[help1y][help1x]), help1x * cellsize + textCenterWidth, help1y * cellsize + textCenterHeight, 20, RAYWHITE);
                DrawRectangle(help2x * cellsize + StageCenterWidth, help2y * cellsize + StageCenterHeight, cellsize, cellsize, BLUE);
                DrawText(TextFormat("%c", stage[help2y][help2x]), help2x * cellsize + textCenterWidth, help2y * cellsize + textCenterHeight, 20, RAYWHITE);
            }
        }
        if ((timerlifetime - 3600 * (timerlifetime/3600)) / 60 > 9)
            DrawText(TextFormat("Time: %i:%i", timerlifetime/3600, (timerlifetime - 3600 * (timerlifetime/3600)) / 60), GetScreenWidth() - 350, 50, 35, BLACK);
        else DrawText(TextFormat("Time: %i:0%i", timerlifetime/3600, (timerlifetime - 3600 * (timerlifetime/3600)) / 60), GetScreenWidth() - 350, 50, 35, BLACK);
        DrawText(TextFormat("SCORE: %i", Highscore), 180, 50, 35, BLACK);
        DrawText("Press H for hint", GetScreenWidth() - 270, GetScreenHeight() - 50, 25, LIGHTGRAY);
    }

    DrawText(TextFormat("mid1: %i;%i, midp2: %i;%i", midp1y, midp1x, midp2y, midp2x), 400, 100, 25, BLACK);

    if (shufflestate == 1){
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.6));
        DrawRectangle(300, 300, 600, 400, WHITE);
        DrawText("There are no matchable pairs left. Re-Shuffling", 330, 450, 50, BLACK);
    }
}


void UnloadGameplayScreen(){
    for (int i = 0; i < 8; i++) delete[] stage[i];
    delete[] stage;
}

int FinishGameplayScreen(){
    return finishGame;
}