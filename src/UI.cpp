#include "Enemy.h"
#include "Item.h"
#include "Json.h"
#include "Location.h"
#include "Player.h"
#include "Stats.h"
#include <Game.h>
#include <Random.h>
#include <UI.h>
#include <curses.h>
#include <menu.h>
#include <string>
#include <utility>
#include <vector>
void UI::GenerateRestUI() {
  Game::ui.Clear();
  GenerateWindow(Actions_Window, 8, COLS - 2, LINES - 9, 1);
  GenerateWindow(Inventory_Window, LINES - (Actions_Window.getHeight()) - 2, 20,
                 1, 1);

  std::vector<std::string> Options = {"INVENTORY", "FIGHT/TAKE", "RUN/NEXT",
                                      "SAVE GAME/EXIT"};
  std::string str = "INVENTORY";
  Inventory_Window.Print_Window_Title(str);

  GenerateWindow(Game_Window, LINES - Actions_Window.getHeight() - 2,
                 COLS - Inventory_Window.getWidth() - 2, 1,
                 Inventory_Window.getWidth() + 1);
  int selection = GenerateRestRandom();
  PrintRestRandom(Game_Window, selection);
  while (Game::game_state == Game::GameState::Rest) {
    wclear(Game_Window.getWindow());
    wattron(Game_Window.getWindow(), COLOR_PAIR(1));
    Game_Window.Box(0, 0);
    wattroff(Game_Window.getWindow(), COLOR_PAIR(1));
    PrintPlayerDetails(Game_Window);

    Menu *Action_menu = new Menu();
    Action_menu->Create(Actions_Window.getWindow(), 1, 2, 3,
                        Actions_Window.getWidth() - 4, 1, 4, Options);
    Actions_Window.setMenu(*Action_menu);
    mvwprintw(Game_Window.getWindow(), 0, 0, "%d", selection);
    PrintRestRandom(Game_Window, selection);

    std::string selec = Action_menu->Handle_Input();

    if (selec == Options[0]) {
      Game::INVENTORY = Game::player.getInventory();
      std::vector<std::string> INVENTORY;
      for (const auto &item : Game::INVENTORY) {
        INVENTORY.push_back(item.Name);
      }
      Menu inventory_menu;
      inventory_menu.Create(Inventory_Window.getWindow(), 3, 1,
                            Inventory_Window.getHeight() - 4,
                            Inventory_Window.getWidth() - 1,
                            Inventory_Window.getHeight() - 4, 1, INVENTORY);

      std::string item_name = inventory_menu.Handle_Input();

      std::unordered_map<Slot, Item> Player_equipment =
          Game::player.getEquipment();
      std::vector<std::string> inventory_options = {"USE/EQUIP", "DISCARD"};
      for (auto equipment : Player_equipment) {
        if (equipment.second.Name == item_name) {
          inventory_options[0] = std::string("UNEQUIP");
        }
      }
      Menu item_menu;
      item_menu.Create(
          Inventory_Window.getWindow(), Inventory_Window.getHeight() - 4, 1, 4,
          inventory_options[0].size() + 2, 2, 1, inventory_options);
      std::string item_option = item_menu.Handle_Input();
      if (item_option == "USE/EQUIP") {
        Item I = Game::player.getItembyName(item_name);
        Game::player.equipItem(I);
      }
      if (item_option == "UNEQUIP") {
        Item I = Game::player.getItembyName(item_name);
        Game::player.unequipItem(I.Item_slot);
      }
      if (item_option == "DISCARD") {
        Item I = Game::player.getItembyName(item_name);
        Game::player.DiscardItemFromInventory(I);
      }
      item_menu.Destroy();
      inventory_menu.Destroy();
    }
    if (selec == Options[1]) {
      if (selection == 1) {
        Game::ui.Play_Game_Generated = false;
        Game::game_state = Game::GameState::Play_Game;
        return;
      }
      if (selection == 2) {
        Game::player.addItemToInventory(Game::current_Item);
      }
      selection = Random::Random_Number(1, 2);
    }
    if (selec == Options[2]) {
      selection = GenerateRestRandom();
    }
    if (selec == Options[3]) {
      Json json;
      json.FromObject<Player>(Game::player);
      json.Write(Game::player.getName() + ".json");
      Game::game_state = Game::End_Game;
    }
    Action_menu->Refresh();
  }
  Game::game_state = Game::End_Game;
}
void UI::PrintPlayerDetails(Window &window) {
  int max_height = window.getHeight();
  int max_width = window.getWidth();
  WINDOW *win = window.getWindow();
  std::string E;
  Stats player_stats = Game::player.getStats();
  std::unordered_map<Slot, Item> equipment = Game::player.getEquipment();
  wattron(win, COLOR_PAIR(4));
  E = "PLAYER : " + Game::player.getName();
  mvwprintw(win, 1, 1, "%s", E.c_str());
  E = "HP : " + std::to_string(player_stats.current_HP) + "/" +
      std::to_string(player_stats.max_HP);
  mvwprintw(win, 2, 1, "%s", E.c_str());
  E = "STAMINA : " + std::to_string(player_stats.current_Stamina) + "/" +
      std::to_string(player_stats.max_Stamina);
  mvwprintw(win, 3, 1, "%s", E.c_str());
  E = "ATTACK : " + std::to_string(player_stats.Attack);
  mvwprintw(win, 4, 1, "%s", E.c_str());
  E = "DEFENSE : " + std::to_string(player_stats.Defense);
  mvwprintw(win, 5, 1, "%s", E.c_str());
  E = "HP_REGAIN : " + std::to_string(player_stats.HP_regain);
  mvwprintw(win, 6, 1, "%s", E.c_str());
  E = "STAMINA_REGAIN : " + std::to_string(player_stats.Stamina_regain);
  mvwprintw(win, 7, 1, "%s", E.c_str());

  E = "EQUIPMENT";
  mvwprintw(win, 8, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());

  auto p = equipment.find(Slot::Head);
  std::string St;
  int l = 10;

  E = "HEAD : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  p = equipment.find(Slot::Torso);
  l++;
  E = "TORSO : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  p = equipment.find(Slot::Hands);
  l++;
  E = "HANDS : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  p = equipment.find(Slot::Legs);
  l++;
  E = "LEGS : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  p = equipment.find(Slot::Left_Hand);
  l++;
  E = "LEFT HAND : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  p = equipment.find(Slot::Right_Hand);
  l++;
  E = "RIGHT HAND : ";
  if (p != equipment.end()) {
    Item item = p->second;
    E += item.Name;
    St = GetStatString(item);
  } else {
    E += "Empty";
  }
  mvwprintw(win, l, (max_width / 2 - (E.size() / 2)), "%s", E.c_str());
  l++;
  if (St.size() > 0) {
    mvwprintw(win, l, (max_width / 2 - (St.size() / 2)), "%s", St.c_str());
    l++;
  }
  St = "";

  wattroff(win, COLOR_PAIR(4));
  window.Refresh();
}

void UI::PrintRestRandom(Window &window, int selection) {
  WINDOW *win = window.getWindow();
  // 1 for Enemy , 2 for Item
  std::string S, E;
  switch (selection) {
  case 2:
    S = "You found item : " + Game::current_Item.Name +
        ",Item type : " + std::to_string(Game::current_Item.Class);
    mvwprintw(win, window.getHeight() - 3, 1, "%s", S.c_str());
    E = GetStatString(Game::current_Item);
    mvwprintw(win, window.getHeight() - 2, 1, "%s", E.c_str());
    break;
  case 1:
    S = "You Encountered : " + Game::current_Enemy.getName() + " of level " +
        std::to_string(Game::current_Enemy.getLevel());
    mvwprintw(win, window.getHeight() - 3, 1, "%s", S.c_str());
    E = GetCurrentEnemyStatString();
    mvwprintw(win, window.getHeight() - 2, 1, "%s", E.c_str());
    break;
  }
  window.Refresh();
}
std::string UI::GetCurrentEnemyStatString() {
  Stats S = Game::current_Enemy.getStats();
  std::string str;
  if (S.max_HP != 0) {
    str += "M_HP : " + std::to_string(S.max_HP);
  }
  if (S.max_Stamina != 0) {
    str += "M_STA : " + std::to_string(S.max_HP);
  }
  if (S.Attack != 0) {
    str += "ATT : " + std::to_string(S.Attack);
  }
  if (S.Defense != 0) {
    str += "DEF : " + std::to_string(S.Defense);
  }
  if (S.HP_regain != 0) {
    str += "HP_R : " + std::to_string(S.HP_regain);
  }
  if (S.Stamina_regain != 0) {
    str += "ST_R : " + std::to_string(S.Stamina_regain);
  }
  return str;
}

std::string UI::GetStatString(Item &item) {
  Stats S = item.Stat_Change;
  std::string str;
  if (S.max_HP != 0) {
    str += "M_HP : " + std::to_string(S.max_HP);
  }
  if (S.max_Stamina != 0) {
    str += "M_STA : " + std::to_string(S.max_HP);
  }
  if (S.Attack != 0) {
    str += "ATT : " + std::to_string(S.Attack);
  }
  if (S.Defense != 0) {
    str += "DEF : " + std::to_string(S.Defense);
  }
  if (S.HP_regain != 0) {
    str += "HP_R : " + std::to_string(S.HP_regain);
  }
  if (S.Stamina_regain != 0) {
    str += "ST_R : " + std::to_string(S.Stamina_regain);
  }
  return str;
}
void UI::GeneratePlayGameUI(std::vector<std::string> Options,
                            std::vector<std::string> INVENTORY) {
  if (Game::ui.Play_Game_Generated)
    return;
  refresh();
  int action_menu_width = 12;
  GenerateWindow(Main_Window, 0, 0, 0, 0);

  GenerateWindow(Actions_Window, 8, COLS - 2, LINES - 9, 1);

  GenerateWindow(Inventory_Window, LINES - (Actions_Window.getHeight()) - 2, 20,
                 1, 1);

  Menu inventory_menu;
  inventory_menu.Create(Inventory_Window.getWindow(), 3, 1,
                        Inventory_Window.getHeight() - 4,
                        Inventory_Window.getWidth() - 1,
                        Inventory_Window.getHeight() - 4, 1, INVENTORY);
  Inventory_Window.setMenu(inventory_menu);

  std::string str = "INVENTORY";
  Inventory_Window.Print_Window_Title(str);

  GenerateWindow(Status_Window, 11, COLS - (Inventory_Window.getWidth()) - 2, 1,
                 (Inventory_Window.getWidth() + 1));

  GenerateWindow(
      Game_Window,
      LINES - (Status_Window.getHeight() + Actions_Window.getHeight() + 2),
      (COLS - Inventory_Window.getWidth() - 2), Status_Window.getHeight() + 1,
      Inventory_Window.getWidth() + 1);

  Game::game_row =
      Random::Random_Number(5, (Game_Window.getHeight() / 2) - 3) * 2;
  Game::game_col =
      Random::Random_Number(5, (Game_Window.getWidth() / 2) - 2) * 2;

  Generate_Grid(Game_Window.getWindow(), Game::game_row, Game::game_col);
  GenerateStatusWindow();
  Game_Window.Refresh();
  refresh();
  bool block = false;
  if (Actions_Window.getWindow()) {
    curs_set(1);
    while (Game::player.getStats().current_HP != 0) {
      // Actions_Window.Refresh();
      Menu action_menu;
      action_menu.Create(Actions_Window.getWindow(), 1, 1, 6, action_menu_width,6, 1, Options);
      Actions_Window.setMenu(action_menu);
      // 0 for did no move , 1 for Moved , >1 for damage
      std::pair<std::string, int> P = Game::current_Enemy.Turn(Game::player);
      EnemyTurnString = P.first;
      std::string Selected = action_menu.Handle_Input();
      if (Selected == Game::ACTION_OPTIONS[4]) {
        inventory_menu.Refresh();
        std::string s = inventory_menu.Handle_Input();
        PlayerTurnString = "Player equipped/used item " + s;
      }
      if (Selected == Game::ACTION_OPTIONS[3]) {
        MoveInGrid(2);
      }
      if (Selected == Game::ACTION_OPTIONS[0]) {
        std::unordered_map<std::string, std::string> Attack_Options = {
            {"R - LIGHT", "25"},
            {"R - HEAVY", "35"},
            {"L - LIGHT", "15"},
            {"L - HEAVY", "35"},
            {"BACK", "0"}};
        Menu attack_menu;
        if (block) {
          Attack_Options.insert(std::make_pair("GUARD COUNTER", "47"));
          block = false;
        }
        attack_menu.Create(Actions_Window.getWindow(), 1, 1 + action_menu_width,
                           6, 16, Attack_Options);
        attack_menu.Refresh();
        float multiplier = 0.0f;
        std::string attack_select = attack_menu.Handle_Input();
        auto iter = Attack_Options.find(attack_select);
        if (iter != Attack_Options.end()) {
          auto option = iter->first;
          if (iter->second == "0")
          {
            attack_menu.Destroy();
            Actions_Window.Refresh();
            continue;
          }
          multiplier = std::stof(iter->second) / 10;
        }
        int damage = Game::player.Attack(Game::current_Enemy, multiplier);
        PlayerTurnString = Game::player.getName() + " attacked the enemy " +
                           Game::current_Enemy.getName() + " for " +
                           std::to_string(damage) + " damage";
        attack_menu.Destroy();
        wattron(Actions_Window.getWindow(), COLOR_PAIR(1));
        Actions_Window.Box(0, 0);
        wattroff(Actions_Window.getWindow(), COLOR_PAIR(1));
        action_menu.Refresh();
        Actions_Window.Refresh();
      }
      if (Selected == Game::ACTION_OPTIONS[1]) {
        PlayerTurnString = "Player tried to dodge the attack.";
        int r = Random::Random_Number(0, 1);
        if (P.second > 1) {
          if (r == 0) {
            PlayerTurnString += "And succeeded!";
            if (P.second > 1)
              Game::player.RecoverHP(P.second);
          } else
            PlayerTurnString += "But Failed.";
        }
      }
      if (Selected == Game::ACTION_OPTIONS[2]) {
        PlayerTurnString = "Player tried to block the damage.";
        if (P.second > 1) {
          block = true;
          int BlockedDamage = Game::player.getStats().Defense / 4 +
                              Game::player.getStats().HP_regain;
          Game::player.RecoverHP(BlockedDamage);
          PlayerTurnString += "And blocked " +
                              std::to_string(P.second - BlockedDamage) +
                              " damage";
        }
      }
      if (Selected == Game::ACTION_OPTIONS[5]) {
        PlayerTurnString = "Player Rested this turn";
        int R = Game::player.getStats().HP_regain * 2;
        Game::player.RecoverHP(R);
        PlayerTurnString += " and recoverd " + std::to_string(R) + "HP.";
      }
      if (Game::current_Enemy.getStats().current_HP <= 0) {
        std::string str = "ENEMY IS DEAD!";
        GenerateEndScreen(str);
        End_Screen_generated = false;
        inventory_menu.Destroy();
        action_menu.Destroy();
        Game::game_state = Game::Rest;
        break;
      }
      GenerateStatusWindow();
      UpdatePlayerandEnemyPos();
      PrintTurnActions();
      Game_Window.Refresh();
    }
    // wgetch(Game_Window.getWindow());
  }
  Game::ui.Play_Game_Generated = true;
}
int UI::GenerateRestRandom() {
  // 1 for Enemy , 2 for Item
  int selection = Random::Random_Number(1, 2);
  if (selection == 1) {
    int enemy_index = Random::Random_Number(0, Game::Enemy_list.size() - 1);
    Location Zero_Location = {0, 0};
    Stats Zero_Stats = Stats();
    Enemy enemy = Enemy(Game::Enemy_list[enemy_index], Zero_Stats,
                        Zero_Location, Game::player.getLevel());
    Game::current_Enemy = enemy;
  }
  if (selection == 2) {
    Game::current_Item =
        Game::Item_List[Random::Random_Number(0, Game::Item_List.size() - 1)];
  }
  return selection;
}

void UI::PrintTurnActions() {
  wmove(Game_Window.getWindow(), Game_Window.getHeight() - 3, 1);
  wclrtoeol(Game_Window.getWindow());
  wmove(Game_Window.getWindow(), Game_Window.getHeight() - 2, 1);
  wclrtoeol(Game_Window.getWindow());
  mvwprintw(Game_Window.getWindow(), Game_Window.getHeight() - 4, 1, "Turn :");
  mvwprintw(Game_Window.getWindow(), Game_Window.getHeight() - 3, 2,
            "PLAYER Action : %s", PlayerTurnString.c_str());
  mvwprintw(Game_Window.getWindow(), Game_Window.getHeight() - 2, 2,
            "ENEMY Action : %s", EnemyTurnString.c_str());

  PlayerTurnString = "";
  EnemyTurnString = "";
}

void UI::GenerateWindow(Window &win, int height, int width, int start_y,
                        int start_x) {
  if (height == 0 && width == 0 && start_x == 0 && start_y == 0)
    win = Window();
  else {
    win = Window(height, width, start_y, start_x);
  }
  wattron(win.getWindow(), COLOR_PAIR(1));
  win.Box(0, 0);
  wattroff(win.getWindow(), COLOR_PAIR(1));
  win.Refresh();
}

void UI::GenerateMainMenu(std::vector<std::string> &Options) {
  GenerateWindow(Main_Menu_Window, Options.size() + 2, 12,
                 (LINES - Options.size()) / 2, (COLS - Options[1].size()) / 2);

  Menu main_menu;
  main_menu.Create(Main_Menu_Window.getWindow(), 1, 0,
                   Main_Menu_Window.getHeight() - 2,
                   Main_Menu_Window.getWidth(),
                   Main_Menu_Window.getHeight() - 2, 1, Options);
  Main_Menu_Window.setMenu(main_menu);
  std::string Selected = main_menu.Handle_Input();
  if (Selected == "Exit") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    Game::ExitGame();
  }
  if (Selected == "Load") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    Game::game_state = Game::Character_Select;
  }
  if (Selected == "New Game") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    UI::EnterPlayerDetails();
    Stats newPlayerStat;
    newPlayerStat.max_HP = 75;
    newPlayerStat.current_HP = 75;
    Game::player.setStats(newPlayerStat);
    Game::game_state = Game::Play_Game;
  }
}

void UI::GenerateCharaterSelect(std::vector<std::string> &Characters) {
  if (Character_Select_generated)
    return;
  Characters.push_back("Exit");
  GenerateWindow(Character_Select_Window, Characters.size() + 2,
                 Characters[1].size() + 3, (LINES - Characters.size()) / 2,
                 (COLS - Characters[1].size()) / 2);
  mvprintw(0, 0, "Select file to load from :");
  refresh();
  Menu Character_menu;
  set_menu_mark(Character_menu.getMENU(), ">");
  curs_set(1);
  Character_menu.Create(Character_Select_Window.getWindow(), 1, 0,
                        Character_Select_Window.getHeight() - 2,
                        Character_Select_Window.getWidth(),
                        Character_Select_Window.getHeight() - 2, 1, Characters);
  Character_Select_Window.setMenu(Character_menu);
  Character_Select_generated = true;
  std::string Selected = Character_menu.Handle_Input();
  if (Selected == "Exit") {
    Game::ExitGame();
  } else {
    std::string file_name = Selected + ".json";
    Json json;
    json.Read(file_name);
    json.ToObject<Player>(Game::player);
  }
  Character_Select_Window.DestroyWindow();
  Character_menu.Destroy();
}

void UI::Clear() {
  for (auto win : {New_Game_Window, Main_Window, Actions_Window,
                   Inventory_Window, Status_Window, Game_Window,
                   Main_Menu_Window, Character_Select_Window, End_Screen}) {
    // win.DestroyMenu();
    win.DestroyWindow();
  }
}

void UI::GenerateEndScreen(std::string &S) {
  if (End_Screen_generated)
    return;
  GenerateWindow(End_Screen, 3, COLS - 2, (LINES / 2) - 1, 1);
  std::string str = S;
  End_Screen.Print_Window_Title(str);
  End_Screen_generated = true;
  curs_set(0);
  wgetch(End_Screen.getWindow());
  curs_set(1);
}

void UI::Generate_Grid(WINDOW *window, int row, int col) {
  Game::col_beg = ((Game_Window.getWidth() / 2) - (col / 2));
  Game::row_beg = ((Game_Window.getHeight() / 2) - (row / 2));

  wattron(window, COLOR_PAIR(1));

  // Draw top and bottom borders
  mvwhline(window, Game::row_beg, Game::col_beg, ACS_HLINE, col);
  mvwhline(window, Game::row_beg + row, Game::col_beg, ACS_HLINE, col);

  // Draw left and right borders
  mvwvline(window, Game::row_beg, Game::col_beg, ACS_VLINE, row);
  mvwvline(window, Game::row_beg, Game::col_beg + col, ACS_VLINE, row);

  mvwaddch(window, Game::row_beg, Game::col_beg, ACS_ULCORNER);
  mvwaddch(window, Game::row_beg + row, Game::col_beg, ACS_LLCORNER);
  mvwaddch(window, Game::row_beg, Game::col_beg + col, ACS_URCORNER);
  mvwaddch(window, Game::row_beg + row, Game::col_beg + col, ACS_LRCORNER);

  wattroff(window, COLOR_PAIR(1));

  // Draw horizontal lines
  for (int i = 1; i < row; i += 2) {
    if (i == 1 || i == row - 1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg, ACS_LTEE);
      mvwhline(window, Game::row_beg + i, Game::col_beg + 1, ACS_HLINE,
               col - 2);
      mvwaddch(window, Game::row_beg + i, Game::col_beg + col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1));
    } else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg, ACS_LTEE);
      wattroff(window, COLOR_PAIR(1));
      mvwhline(window, Game::row_beg + i, Game::col_beg + 1, ACS_HLINE,
               col - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg + col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1));
    }
  }

  // Draw vertical lines
  for (int i = 1; i < col; i += 2) {
    if (i == 1 || i == col - 1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg, Game::col_beg + i, ACS_TTEE);
      mvwvline(window, Game::row_beg + 1, Game::col_beg + i, ACS_VLINE,
               row - 2);
      mvwaddch(window, Game::row_beg + row, Game::col_beg + i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    } else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg, Game::col_beg + i, ACS_TTEE);
      wattroff(window, COLOR_PAIR(1));
      mvwvline(window, Game::row_beg + 1, Game::col_beg + i, ACS_VLINE,
               row - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + row, Game::col_beg + i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    }
  }

  // Draw intersections
  for (int i = 1; i < row; i += 2) {
    for (int j = 1; j < col; j += 2) {
      if (i == 1 || i == row - 1 || j == 1 || j == col - 1) {
        wattron(Game_Window.getWindow(), COLOR_PAIR(1));
        mvwaddch(Game_Window.getWindow(), Game::row_beg + i, Game::col_beg + j,
                 ACS_PLUS);
        wattroff(Game_Window.getWindow(), COLOR_PAIR(1));
      } else {
        mvwaddch(Game_Window.getWindow(), Game::row_beg + i, Game::col_beg + j,
                 ACS_PLUS);
      }
    }
  }
  // generate Obstacles
  int obstacle_count = Random::Random_Number(row / 2, col / 2) / 2;
  wattron(window, A_BOLD | COLOR_PAIR(3));
  for (int i = 0; i < obstacle_count; i += 1) {
    int x = Random::Random_Number(1, (col - 1) / 2) * 2;
    int y = Random::Random_Number(1, (row - 1) / 2) * 2;
    Location L;
    L.x = Game::col_beg + x;
    L.y = Game::row_beg + y;
    Game::Obstacle_Locations.push_back(L);
    // Printing the obstacles
    mvwprintw(window, Game::row_beg + y, Game::col_beg + x, "0");
  }
  wattroff(window, A_BOLD | COLOR_PAIR(3));
  Game::player.setPreviousLocation(Game::player.getLocation());
  Game::player.setPlayerLocation(
      1, Random::Random_Number(1, (Game::game_row - 1) / 2));
  if (!Game::isObstacleLocation(Game::player.getPlayerLocation())) {
    wattron(Game_Window.getWindow(), COLOR_PAIR(5) | A_BLINK | A_BOLD);
    mvwprintw(window, Game::row_beg + (Game::player.getPlayerLocation().y * 2),
              Game::col_beg + Game::player.getPlayerLocation().x + 1, "P");
    wattroff(Game_Window.getWindow(), COLOR_PAIR(5) | A_BLINK | A_BOLD);
  }

  // Game::current_Enemy.setEnemyLocation(Game::game_col -
  // 2,Random::Random_Number(1, ((Game::game_row - 1) /2) ) * 2);
  Game::current_Enemy.setPreviousLocation(Game::current_Enemy.getLocation());
  Game::current_Enemy.setEnemyLocation(
      (Game::game_col - 2) / 2,
      Random::Random_Number(1, (Game::game_row - 1) / 2));
  if (!Game::isObstacleLocation(Game::current_Enemy.getEnemyLocation())) {
    wattron(Game_Window.getWindow(), COLOR_PAIR(6));
    mvwprintw(
        window, Game::row_beg + (Game::current_Enemy.getEnemyLocation().y * 2),
        Game::col_beg + Game::current_Enemy.getEnemyLocation().x * 2, "E");
    wattroff(Game_Window.getWindow(), COLOR_PAIR(6));
  }
}

void UI::UpdatePlayerandEnemyPos() {
  // Clear previous player position
  mvwprintw(Game_Window.getWindow(),
            Game::row_beg + (Game::player.getPreviousLocation().y * 2),
            Game::col_beg + Game::player.getPreviousLocation().x * 2, " ");

  // Clear previous enemy position
  mvwprintw(Game_Window.getWindow(),
            Game::row_beg + (Game::current_Enemy.getPreviousLocation().y * 2),
            Game::col_beg + Game::current_Enemy.getPreviousLocation().x * 2,
            " ");

  // Update player position
  wattron(Game_Window.getWindow(), COLOR_PAIR(5) | A_BLINK | A_BOLD);
  mvwprintw(Game_Window.getWindow(),
            Game::row_beg + (Game::player.getPlayerLocation().y * 2),
            Game::col_beg + Game::player.getPlayerLocation().x * 2, "P");
  wattroff(Game_Window.getWindow(), COLOR_PAIR(5) | A_BLINK | A_BOLD);
  Game::player.setPlayerLocation(Game::player.getLocation());

  // Update enemy position
  wattron(Game_Window.getWindow(), COLOR_PAIR(6));
  mvwprintw(Game_Window.getWindow(),
            Game::row_beg + (Game::current_Enemy.getEnemyLocation().y * 2),
            Game::col_beg + Game::current_Enemy.getEnemyLocation().x * 2, "E");
  wattroff(Game_Window.getWindow(), COLOR_PAIR(6));
  Game::current_Enemy.setPreviousLocation(Game::current_Enemy.getLocation());

  // Refresh the window to display the changes
  wrefresh(Game_Window.getWindow());
}

void UI::GenerateStatusWindow() {
  wclear(Status_Window.getWindow());
  wattron(Status_Window.getWindow(), COLOR_PAIR(1));
  Status_Window.Box(0, 0);
  wattroff(Status_Window.getWindow(), COLOR_PAIR(1));
  wattron(Status_Window.getWindow(), COLOR_PAIR(4));
  int w_max_x = Status_Window.getWidth(), w_max_y = Status_Window.getHeight();
  int beg_x = 1, beg_y = 1;
  mvwprintw(Status_Window.getWindow(), beg_x, beg_y, "PLAYER : %s",
            Game::player.getName().c_str());

  Stats player_stats = Game::player.getStats();
  std::string E = "HP:[" + std::to_string(player_stats.current_HP) + "/" +
                  std::to_string(player_stats.max_HP) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 2, beg_x, "%s", E.c_str());
  int j = 0;
  for (int i = player_stats.current_HP; i > 0; i = i - 10) {
    mvwaddch(Status_Window.getWindow(), beg_y + 2, beg_x + E.size() + j,
             ACS_DIAMOND);
    j += 1;
  }

  E = "STAMINA:[" + std::to_string(player_stats.current_Stamina) + "/" +
      std::to_string(player_stats.max_HP) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 4, beg_x, "%s", E.c_str());
  j = 1;
  for (int i = player_stats.current_Stamina; i > 0; i = i - 10) {
    mvwaddch(Status_Window.getWindow(), beg_y + 4, beg_x + E.size() + j,
             ACS_CKBOARD);
    j += 1;
  }
  E = "LEVEL:[" + std::to_string(Game::player.getLevel()) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 5, beg_x, "%s", E.c_str());

  E = "ATTACK:[" + std::to_string(Game::player.getStats().Attack) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 6, beg_x, "%s", E.c_str());

  E = "DEFENSE:[" + std::to_string(Game::player.getStats().Defense) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 7, beg_x, "%s", E.c_str());

  E = "PLAYER LOCATION : x = " +
      std::to_string(Game::player.getPlayerLocation().x) +
      ", y = " + std::to_string(Game::player.getPlayerLocation().y);
  mvwprintw(Status_Window.getWindow(), beg_y + 8, beg_x, "%s", E.c_str());
  E = "ENEMY :" + Game::current_Enemy.getEnemyName();
  mvwprintw(Status_Window.getWindow(), beg_x, w_max_x - E.size() - 1, "%s",
            E.c_str());

  Stats Enemy_Stats = Game::current_Enemy.getEnemyStats();
  E = "HP :[" + std::to_string(Enemy_Stats.current_HP) + "/" +
      std::to_string(Enemy_Stats.max_HP) + "]";
  mvwprintw(Status_Window.getWindow(), beg_x + 2, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "STAMINA : [" + std::to_string(Enemy_Stats.current_Stamina) + "/" +
      std::to_string(Enemy_Stats.max_Stamina) + "]";
  mvwprintw(Status_Window.getWindow(), beg_x + 4, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "LEVEL : " + std::to_string(Game::current_Enemy.getLevel());
  mvwprintw(Status_Window.getWindow(), beg_x + 5, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "ATTACK:[" + std::to_string(Game::current_Enemy.getStats().Attack) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 6, w_max_x - E.size() - 1, "%s",
            E.c_str());

  E = "DEFENSE:[" + std::to_string(Game::current_Enemy.getStats().Defense) +
      "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 7, w_max_x - E.size() - 1, "%s",
            E.c_str());

  E = "ENEMY LOCATION : x = " +
      std::to_string(Game::current_Enemy.getEnemyLocation().x) +
      ", y = " + std::to_string(Game::current_Enemy.getEnemyLocation().y);
  mvwprintw(Status_Window.getWindow(), beg_y + 8, w_max_x - E.size() - 1, "%s",
            E.c_str());
  wattroff(Status_Window.getWindow(), COLOR_PAIR(4));
  Status_Window.Refresh();
}
void UI::MoveInGrid(int increment) {
  int c;
  bool enterPressed = false;
  int cur_X = Game::col_beg + (Game::player.getPlayerLocation().x * 2);
  int cur_Y = Game::row_beg + (Game::player.getPlayerLocation().y * 2);
  Location CurrentLocation;
  CurrentLocation.y = cur_Y;
  CurrentLocation.x = cur_X;
  Location newLocation = Game::player.getPlayerLocation();
  wmove(Game_Window.getWindow(), CurrentLocation.y, CurrentLocation.x);
  while ((c = wgetch(Game_Window.getWindow())) != KEY_F(1)) {
    switch (c) {
    case KEY_DOWN:
      newLocation = {CurrentLocation.x, CurrentLocation.y + increment};
      if (cur_Y + increment >= Game::row_beg + 2 &&
          cur_Y + increment <= Game::row_beg + Game::game_row - 2 &&
          Game::isObstacleLocation(newLocation) == false &&
          !Game::isEnemyLocation(newLocation)) {
        cur_Y += increment;
        CurrentLocation.y = cur_Y;
      }
      break;
    case KEY_UP:
      newLocation = {CurrentLocation.x, CurrentLocation.y - increment};
      if (cur_Y - increment >= Game::row_beg + 2 &&
          cur_Y <= Game::row_beg + Game::game_row - 2 &&
          Game::isObstacleLocation(newLocation) == false &&
          !Game::isEnemyLocation(newLocation)) {
        cur_Y -= increment;
        CurrentLocation.y = cur_Y;
      }
      break;
    case KEY_RIGHT:
      newLocation = {CurrentLocation.x + increment, CurrentLocation.y};
      if (cur_X + increment >= Game::col_beg + 2 &&
          cur_X + increment <= Game::col_beg + Game::game_col - 2 &&
          Game::isObstacleLocation(newLocation) == false &&
          !Game::isEnemyLocation(newLocation)) {
        cur_X += increment;
        CurrentLocation.x = cur_X;
      }
      break;
    case KEY_LEFT:
      newLocation = {CurrentLocation.x - increment, CurrentLocation.y};
      if (cur_X - increment >= Game::col_beg + 2 &&
          cur_X - increment <= Game::col_beg + Game::game_col - 2 &&
          Game::isObstacleLocation(newLocation) == false &&
          !Game::isEnemyLocation(newLocation)) {
        cur_X -= increment;
        CurrentLocation.x = cur_X;
      }
      break;
    default:
    case 10:
    case KEY_ENTER:
      enterPressed = true;
      newLocation = {(CurrentLocation.x - Game::col_beg) / 2,
                     (CurrentLocation.y - Game::row_beg) / 2};
      // mvwprintw(Game_Window.getWindow(), 0, 0,"Current Position = %d , %d and
      // isObstacleLocation = %d",newLocation.y,
      // newLocation.x,Game::isObstacleLocation(CurrentLocation));
      Game::player.setPreviousLocation(Game::player.getPlayerLocation());
      Game::player.setPlayerLocation(newLocation);
    }
    if (enterPressed) {
      PlayerTurnString = "Player moved from " +
                         std::to_string(Game::player.getPreviousLocation().x) +
                         "," +
                         std::to_string(Game::player.getPreviousLocation().y) +
                         " to " + std::to_string(Game::player.getLocation().x) +
                         "," + std::to_string(Game::player.getLocation().y);
      break;
    }
    wmove(Game_Window.getWindow(), CurrentLocation.y, CurrentLocation.x);
    Game_Window.Refresh();
  }
}
void UI::EnterPlayerDetails() {
  std::string str = "Enter Your Name :";
  GenerateWindow(New_Game_Window, 5, COLS - 4,
                 (LINES - New_Game_Window.getHeight()) / 2, 2);
  New_Game_Window.Print_Window_Title(str);
  echo();
  curs_set(2);
  wattron(New_Game_Window.getWindow(), A_BLINK);
  New_Game_Window.Refresh();
  char name[50];
  mvwgetstr(New_Game_Window.getWindow(), 3, 1, name);
  noecho();
  Game::player.setName(std::string(name));
  // mvprintw(1, 0, "Player name : %s", Game::player.getName().c_str());
  wattroff(New_Game_Window.getWindow(), A_BLINK);
  refresh();
  // getch();
  New_Game_Window.DestroyWindow();
}
