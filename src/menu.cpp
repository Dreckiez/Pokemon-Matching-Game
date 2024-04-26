#include "raylib.h"
#include "screens.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;

int finishMenu = 0;
int menutitleposx = 0, menutitleposy = 0;
int menucurpos = 0;
int menuoptionx = 0, menuoptiony = 0;
int menustate = 0;                          //Determine what screen will be draw: 0 = login screen, 1 = register screen, 2 = menu screen
int logintitlex = 0, logintitley = 0;
int inputbar = 0, inputstate = -1;
const char *menu[] ={"Play", "Leaderboard", "Exit"};
string loginusername, loginpassword, regusername, regpassword, username;

//Set the initial values for some variables
void InitTitleScreen(){
    logintitlex = GetScreenWidth()/2 - MeasureTextEx(font, "LOGIN", 100, 0).x - 20;
    logintitley = GetScreenHeight()/2 - MeasureTextEx(font, "LOGIN", 100, 0).y - 180;
    menutitleposx = GetScreenWidth()/2 - MeasureTextEx(font, "Pikachu", 125 , 0).x - 200;
    menutitleposy = GetScreenHeight()/2 + 30 - MeasureTextEx(font, "Pikachu", 125 , 0).y;
    menuoptionx = menutitleposx + 550;
    menuoptiony = menutitleposy - 30;

    finishMenu = 0;
}

void UpdateTitleScreen(){
    if (menustate == 0){ // Login screen
        int key = GetCharPressed();
        while (key > 0) //input character when a key is pressed
        {
            if (inputbar == 0){
                if ((key >= 32) && (key <= 125)) loginusername += (char)key;
            }
            else if (inputbar == 1){
                if ((key >= 32) && (key <= 125)) loginpassword += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){ //deleteing character in string when pressing backspace
            if (inputbar == 0){
                if (!loginusername.empty()) loginusername.pop_back();
            }
            else if (inputbar == 1){
                if (!loginpassword.empty()) loginpassword.pop_back();
            }
        }
        if (IsKeyPressed(KEY_TAB)){ //switching between username bar, password bar, login button and register option
            inputbar++;
            if (inputbar == 4) inputbar = 0;
        }
        if (IsKeyPressed(KEY_ENTER) && inputbar == 3){ //Change to register screen
            menustate = 1;
            inputbar = 0;
        } 
        else if (IsKeyPressed(KEY_ENTER) && inputbar == 2){ // If login button is pressed, check if the username and password are correct
            ifstream fin;
            fin.open("Players.txt");
            if(!fin) fin.open("Players.txt");
            string checkuser, checkpass, line;
            while(getline(fin, line)){
                stringstream ss(line);
                getline(ss, checkuser, '|');
                getline(ss, checkpass, '|');
                if (loginusername != checkuser || loginpassword != checkpass) inputstate = 0; // 
                else{
                    inputstate = 2;
                    break;
                }
            }
            fin.close();
            if (inputstate == 2){ // If username and password are correct, change to menu screen
                username = loginusername;
                menustate = 2;
            }
        }

    }
    else if (menustate == 1){ //Register screen
        int key = GetCharPressed();
        while (key > 0) //input character when a key is pressed
        {
            if (inputbar == 0){
                if ((key >= 32) && (key <= 125)) regusername += (char)key;
            }
            else if (inputbar == 1){
                if ((key >= 32) && (key <= 125)) regpassword += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){ //deleteing character in string when pressing backspace
            if (inputbar == 0){
                if (!regusername.empty()) regusername.pop_back();
            }
            else if (inputbar == 1){
                if (!regpassword.empty()) regpassword.pop_back();
            }
        }
        if (IsKeyPressed(KEY_TAB)){ //switching between username bar, password bar and register button
            inputbar++;
            if (inputbar == 3) inputbar = 0;
        }
        if (IsKeyPressed(KEY_ENTER) && inputbar == 2){ //Check if username is taken
            inputstate = 2;
            ifstream fin; 
            fin.open("Players.txt");
            if(!fin) fin.open("Players.txt");
            string checkuser, line;
            while(getline(fin, line)){
                stringstream ss(line);
                getline(ss, checkuser, '|');
                if (regusername == checkuser){
                    inputstate = 1;
                    break;
                }
                else inputstate = 2;
            }
            fin.close();
            if (inputstate == 2){ // If username is not taken, save the player username and password to text file
                ofstream fout;
                fout.open("Players.txt", ios::app);
                if (!fout) fout.open("Players.txt", ios::app);
                fout << regusername << '|' << regpassword << '|' << 0 << '\n';
                fout.close();
                username = regusername;
                menustate = 2; //Change to Menu screen
            }
        }
    }
    else if (menustate == 2){ // Main Menu screen
        if (IsKeyPressed(KEY_DOWN)) menucurpos++;
        if (IsKeyPressed(KEY_UP)) menucurpos--;
        if (menucurpos < 0) menucurpos = 2;
        else if (menucurpos > 2) menucurpos = 0;
        if (IsKeyPressed(KEY_ENTER)){
            switch (menucurpos)
            {
                case 0:
                    finishMenu = 2; // Change to Gameplay screen
                    break;
                case 1:
                    finishMenu = 4; // Change to Leaderboard screen
                    break;
                case 2:
                    finishMenu = 3; // Exit the game
                default:
                    break;
            }
            PlaySound(fxChange);
        }
    }
}

void DrawTitleScreen(){
    if (menustate == 0){ // Draw the Login box
        DrawText("Pikachu", menutitleposx, menutitleposy, 125, BLACK);
        for (int i = 0; i < 3; i++){
            DrawText(TextFormat("%s", menu[i]), menuoptionx, menuoptiony + i * 80, 50, BLACK);
        }
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.6));

        DrawRectangle(logintitlex - 202, logintitley - 57, 700, 590, WHITE);
        DrawRectangleLines(logintitlex - 202, logintitley - 57, 700, 590, BLACK);

        DrawText("LOGIN", logintitlex, logintitley, 100, BLACK);
        DrawText("Username:", logintitlex + 5, logintitley + 110, 25, BLACK);
        DrawRectangle(logintitlex, logintitley + 150, 290, 50, LIGHTGRAY);
        DrawText(TextFormat("%s", &loginusername[0]), logintitlex + 10, logintitley + 163, 25, GRAY);
        DrawRectangleLines(logintitlex, logintitley + 150, 290, 50, GRAY);

        DrawText("Password:", logintitlex + 5, logintitley + 220, 25, BLACK);
        DrawRectangle(logintitlex, logintitley + 260, 290, 50, LIGHTGRAY);
        DrawText(TextFormat("%s", &loginpassword[0]), logintitlex + 10, logintitley + 273, 25, GRAY);
        DrawRectangleLines(logintitlex, logintitley + 260, 290, 50, GRAY);

        DrawRectangle(logintitlex, logintitley + 370, 290, 50, LIGHTGRAY);
        DrawText("Login", logintitlex + 83, logintitley + 372, 45, WHITE);
        DrawRectangleLines(logintitlex, logintitley + 370, 290, 50, LIGHTGRAY);

        DrawText("Don't have an account? Register", logintitlex - 55, logintitley + 450, 25, LIGHTGRAY);
        if (inputbar == 0){
            DrawRectangle(logintitlex, logintitley + 150, 290, 50, GRAY);
            DrawText(TextFormat("%s", &loginusername[0]), logintitlex + 10, logintitley + 163, 25, WHITE);
            DrawRectangleLines(logintitlex, logintitley + 150, 290, 50, DARKGRAY);
        }
        else if (inputbar == 1){
            DrawRectangle(logintitlex, logintitley + 260, 290, 50, GRAY);
            DrawText(TextFormat("%s", &loginpassword[0]), logintitlex + 10, logintitley + 273, 25, WHITE);
            DrawRectangleLines(logintitlex, logintitley + 260, 290, 50, DARKGRAY);
        }
        else if (inputbar == 2){
            DrawRectangle(logintitlex, logintitley + 370, 290, 50, GRAY);
            DrawRectangleLines(logintitlex, logintitley + 370, 290, 50, DARKGRAY);
            DrawText("Login", logintitlex + 83, logintitley + 372, 45, WHITE);
        }
        else {
            DrawText("Don't have an account? Register", logintitlex - 55, logintitley + 450, 25, BLACK);
        }
        if (inputstate == 0)
            DrawText("Incorrect username or password. Please try again", logintitlex - 100, logintitley + 340, 20, RED);
    }
    else if (menustate == 1){ // Draw the Register box
        DrawText("Pikachu", menutitleposx, menutitleposy, 125, BLACK);
        for (int i = 0; i < 3; i++){
            DrawText(TextFormat("%s", menu[i]), menuoptionx, menuoptiony + i * 80, 50, BLACK);
        }
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.6));

        DrawRectangle(logintitlex - 202, logintitley - 57, 700, 590, WHITE);
        DrawRectangleLines(logintitlex - 202, logintitley - 57, 700, 590, BLACK);

        DrawText("Register", logintitlex - 62, logintitley, 100, BLACK);

        DrawText("Username:", logintitlex + 5, logintitley + 110, 25, BLACK);
        DrawRectangle(logintitlex, logintitley + 150, 290, 50, LIGHTGRAY);
        DrawRectangleLines(logintitlex, logintitley + 150, 290, 50, GRAY);
        DrawText(TextFormat("%s", &regusername[0]), logintitlex + 10, logintitley + 163, 25, GRAY);

        DrawText("Password:", logintitlex + 5, logintitley + 220, 25, BLACK);
        DrawRectangle(logintitlex, logintitley + 260, 290, 50, LIGHTGRAY);
        DrawRectangleLines(logintitlex, logintitley + 260, 290, 50, GRAY);
        DrawText(TextFormat("%s", &regpassword[0]), logintitlex + 10, logintitley + 273, 25, WHITE);

        DrawRectangle(logintitlex, logintitley + 370, 290, 50, LIGHTGRAY);
        DrawText("Register", logintitlex + 55, logintitley + 372, 45, WHITE);
        DrawRectangleLines(logintitlex, logintitley + 370, 290, 50, LIGHTGRAY);

        if (inputbar == 0){
            DrawRectangle(logintitlex, logintitley + 150, 290, 50, GRAY);
            DrawRectangleLines(logintitlex, logintitley + 150, 290, 50, DARKGRAY);
            DrawText(TextFormat("%s", &regusername[0]), logintitlex + 10, logintitley + 163, 25, WHITE);
        }
        else if (inputbar == 1) {
            DrawRectangle(logintitlex, logintitley + 260, 290, 50, GRAY);
            DrawRectangleLines(logintitlex, logintitley + 260, 290, 50, DARKGRAY);
            DrawText(TextFormat("%s", &regpassword[0]), logintitlex + 10, logintitley + 273, 25, WHITE);
        }
        else{
            DrawRectangle(logintitlex, logintitley + 370, 290, 50, GRAY);
            DrawRectangleLines(logintitlex, logintitley + 370, 290, 50, DARKGRAY);
            DrawText("Register", logintitlex + 55, logintitley + 372, 45, WHITE);
        }
        if (inputstate == 1)
            DrawText("Username has been taken!", logintitlex - 15, logintitley + 335, 25, RED);
    }
    else if (menustate == 2){ // Draw the Menu screen
        DrawText(TextFormat("Welcome %s,", &username[0]), menutitleposx, menuoptiony, 25, BLACK);

        DrawText("Pikachu", menutitleposx, menutitleposy, 125, BLACK);
        for (int i = 0; i < 3; i++){
            if (menucurpos == i) {
                DrawRectangle(menuoptionx - 20, menuoptiony + i * 80 - 10, MeasureText(menu[i], 50) + 43, MeasureTextEx(font, menu[i], 50, 0).y + 20, BLACK);
                DrawText(TextFormat("%s", menu[i]), menuoptionx, menuoptiony + i * 80, 50, RAYWHITE);
            }
            else DrawText(TextFormat("%s", menu[i]), menuoptionx, menuoptiony + i * 80, 50, BLACK);
        }
    }
}


void UnloadTitleScreen(){

}

int FinishTitleScreen(){
    return finishMenu;
}