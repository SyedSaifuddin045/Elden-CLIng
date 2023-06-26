#ifndef UI_H
#define UI_H
#include "Item.h"
#include <ncurses.h>
#include <Window.h>
#include <Menu.h>

class UI{
  private:
  Window New_Game_Window,Main_Window,Actions_Window,Inventory_Window,Status_Window,Game_Window,Main_Menu_Window,Character_Select_Window,End_Screen;
  int input;
  bool Title_generated,Main_Menu_generated,Character_Select_generated,Play_Game_Generated,End_Screen_generated=false;
  std::string PlayerTurnString , EnemyTurnString;
  friend class Game;
  public:
  void GeneratePlayGameUI(std::vector<std::string> Options,std::vector<std::string>INVENTORY);
  void GenerateWindow(Window &win,int height,int width,int start_y,int start_x);
  void GenerateMainMenu(std::vector<std::string> &Options);
  void GenerateCharaterSelect(std::vector<std::string> &Characters);
  void GenerateEndScreen(std::string& S);
  void Generate_Grid(WINDOW* window, int row, int col);
  void GenerateRestUI(std::vector<std::string> INVENTORY);
  void GenerateStatusWindow();
  void Clear();
  void UpdatePlayerandEnemyPos();
  void PrintTurnActions();
  void PrintPlayerDetails(Window& window);
  std::string GetStatString(Item& item);

void printAsciiArtInCenter(const char* asciiArt) {
    // Get the screen size
    int numRows, numCols;
    getmaxyx(stdscr, numRows, numCols);

    // Split the ASCII art into lines
    std::vector<std::string> lines;
    std::string line;
    for (const char* p = asciiArt; *p != '\0'; p++) {
        if (*p == '\n') {
            lines.push_back(line);
            line.clear();
        } else {
            line += *p;
        }
    }
    lines.push_back(line);

    // Print the ASCII art centered on the screen
    int rowOffset = (numRows - lines.size()) / 2 ;
    int t = (lines[3].size())/2;
    int colOffset = ((numCols/2) - t);
    for (int i = 0; i < lines.size(); i++) {
        mvprintw(rowOffset + i, colOffset,"%s",lines[i].c_str());
    }
}
void MoveInGrid(int increment);
void EnterPlayerDetails();
};
#endif // UI_H
