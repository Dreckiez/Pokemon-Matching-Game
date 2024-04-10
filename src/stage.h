#include "raylib.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>
using namespace std;

void InitStage(char** &stage, int row, int col){
    stage = new char*[row];
    for (int i = 0; i < row; i++) stage[i] = new char[col];
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++) stage[i][j] = (char) ((rand() % (90 - 65 + 1)) + 65);
    }
}

void PrintStage(char** stage, int row, int col){
    int cellsize = 60;
    int ScreenCenterWidth = GetScreenWidth()/2 - (col/2) * cellsize;
    int ScreenCenterHeight = GetScreenHeight()/2 - (row/2) * cellsize;
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            DrawRectangleLines(j * cellsize + ScreenCenterWidth, i * cellsize + ScreenCenterHeight, cellsize, cellsize, LIGHTGRAY);
            DrawText(TextFormat("%c", stage[i][j]), j * cellsize + ScreenCenterWidth, i * cellsize + ScreenCenterHeight, 20, BLACK);
        }
    }
}

void DeleteStage(char** &stage, int row, int col){
    for (int i = 0; i < row; i++) delete[] stage[i];
    delete[] stage;
}