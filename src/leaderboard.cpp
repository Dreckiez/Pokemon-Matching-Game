#include "raylib.h"
#include "screens.h"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

int finishLeaderboard = 0;
int ldbcenterx = 0;
int ldbcentery = 0;
int num = 0;

struct Player{
    string username;
    string pass;
    int score;
};
Player Playersss[100];

void SortList(Player pl[], int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n - i - 1; j++)
            if (pl[j].score < pl[j+1].score){
                Player b = pl[j];
                pl[j] = pl[j+1];
                pl[j+1] = b;
            }
    }
}

void InitOptionsScreen(){
    ifstream fin;
    fin.open("Players.txt");
    if(!fin) fin.open("Players.txt");
    string checkuser, checkpass, line;
    int score;
    while(getline(fin, line)){
        stringstream ss(line);
        getline(ss, checkuser, '|');
        Playersss[num].username = checkuser;
        getline(ss, checkpass, '|');
        Playersss[num].pass = checkpass;
        ss >> score;
        Playersss[num].score = score;
        num++;
    }
    fin.close();

    SortList(Playersss, num);

    ldbcenterx = GetScreenWidth()/2 - MeasureTextEx(font, "LEADERBOARD", 100, 0).x;
    ldbcentery = GetScreenHeight()/2 - MeasureTextEx(font, "LEADERBOARD", 100, 0).y;
}

void UpdateOptionsScreen(){
    if (IsKeyPressed(KEY_ENTER)) finishLeaderboard = 1;
}


void DrawOptionsScreen(){
    DrawText("LEADERBOARD", ldbcenterx + 285, ldbcentery + 60, 100, BLACK);
    if (num < 10){
        for (int i = 0; i < num; i++){
            DrawText(TextFormat("%i", i + 1), ldbcenterx + 340, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText(TextFormat("%s", &Playersss[i].username[0]), ldbcenterx + 480, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText(TextFormat("%i", Playersss[i].score), ldbcenterx + 940, ldbcentery + 200 + i * 50, 23, BLACK);
        }
        for (int i = num; i < 10; i++){
            DrawText("-", ldbcenterx + 340, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText("NULL", ldbcenterx + 480, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText("100", ldbcenterx + 940, ldbcentery + 200 + i * 50, 23, BLACK);
        }
    }
    else {
        for (int i = 0; i < 10; i++){
            DrawText(TextFormat("%i", i + 1), ldbcenterx + 340, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText(TextFormat("%s", &Playersss[i].username[0]), ldbcenterx + 480, ldbcentery + 200 + i * 50, 23, BLACK);
            DrawText(TextFormat("%i", Playersss[i].score), ldbcenterx + 940, ldbcentery + 200 + i * 50, 23, BLACK);
        }
    }
}

void UnloadOptionsScreen(){

}


int FinishOptionsScreen(){
    return finishLeaderboard;
}