#include "raylib.h"
#include "screens.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int finishEnd = 0;
extern int Highscore;
extern int timerlifetime;
extern string username;
int endcenterx = 0;
int endcentery = 0;
int n = 0;
int ldb_idx = 0;
string ldb = "-";

struct Player{
    string username;
    string pass;
    int score;
};
Player Players[100];

void ListSort(Player pl[], int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n - i - 1; j++)
            if (pl[j].score < pl[j+1].score){
                Player b = pl[j];
                pl[j] = pl[j+1];
                pl[j+1] = b;
            }
    }
}

// Read all the players data from text file
void InitEndingScreen(){
    ifstream fin;
    fin.open("Players.txt");
    if(!fin) fin.open("Players.txt");
    string checkuser, checkpass, line;
    int score;
    while(getline(fin, line)){
        stringstream ss(line);
        getline(ss, checkuser, '|');
        Players[n].username = checkuser;
        getline(ss, checkpass, '|');
        Players[n].pass = checkpass;
        ss >> score;
        Players[n].score = score;
        n++;
    }
    fin.close();

    endcenterx = GetScreenWidth()/2 - MeasureTextEx(font, "LEADERBOARD", 100, 0).x;
    endcentery = GetScreenHeight()/2 - MeasureTextEx(font, "LEADERBOARD", 100, 0).y;
    finishEnd = 0;
}

void UpdateEndingScreen(){
    ListSort(Players, n); // Sort the players according to their score

    for (int i = 0; i <= n; i++){ // Update the current player score
        if (username == Players[i].username){
            Players[i].score = Highscore + 60000/(36000-timerlifetime);
            ldb_idx = i + 1;
            ldb = to_string(ldb_idx);
            break;
        }
    }

    if (IsKeyPressed(KEY_ENTER)) // Save the players data to text file and switch to Menu screen
    {
        ofstream fout;
        fout.open("Players.txt");
        if (!fout) fout.open("Players.txt");
        for (int i = 0; i < n; i++){
            fout << Players[i].username << '|' << Players[i].pass << '|' << Players[i].score << '\n';
        }
        fout.close();
        finishEnd = 7;
        PlaySound(fxChange);
    }
}

//Draw the Leaderboard, the score, time bonus and total score 
void DrawEndingScreen(){
    DrawText("LEADERBOARD", endcenterx - 30, endcentery - 230, 100, BLACK);

    for (int i = 0; i < 3; i++){
        DrawText(TextFormat("%i", i + 1), endcenterx, endcentery - 70 + i * 70, 30, BLACK);
        DrawText(TextFormat("%s", &Players[i].username[0]), endcenterx + 140, endcentery - 70 + i * 70, 30, BLACK);
        DrawText(TextFormat("%i", Players[i].score), endcenterx + 600, endcentery - 70 + i * 70, 30, BLACK);
    }

    DrawText(TextFormat("%s", &ldb[0]), endcenterx, endcentery + 150, 40, BLACK);
    DrawText(TextFormat("%s", &username[0]), endcenterx + 140, endcentery + 150, 40, BLACK);
    DrawText(TextFormat("%i", Highscore + 60000/(36000-timerlifetime)), endcenterx + 600, endcentery + 150, 40, BLACK);


    DrawLineEx({(float)endcenterx - 50, (float)endcentery + 200}, {(float)endcenterx + 745, (float)endcentery + 200}, 2, BLACK);
    DrawText("SCORE:", endcenterx + 130, endcentery + 225, 30, BLACK);
    DrawText(TextFormat("%i", Highscore), endcenterx + 530, endcentery + 225, 30, BLACK);
    DrawText("TIME BONUS:", endcenterx + 130, endcentery + 275, 30, BLACK);
    DrawText(TextFormat("%i", 60000/(36000-timerlifetime)), endcenterx + 530, endcentery + 275, 30, BLACK);
    DrawText("TOTAL SCORE:", endcenterx + 130, endcentery + 325, 30, BLACK);
    DrawText(TextFormat("%i", Highscore + 60000/(36000-timerlifetime)), endcenterx + 530, endcentery + 325, 30, BLACK);
}

void UnloadEndingScreen(){
    
}

int FinishEndingScreen(){
    return finishEnd;
}