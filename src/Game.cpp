 #include <Game.h>

static std::vector<std::string> Options = {
  "ATTACK","DODGE","BLOCK","MOVE","INVENTORY","REST"
};
int main(int argc, char *argv[]) 
{
  Initialize();

  /*
  WINDOW *game_window,*main_options_window;
  game_window=newwin(0,0,0,0);
  box(game_window,0,0);
  wgetch(game_window);

  main_options_window = newwin(LINES - (LINES-20),COLS-2,LINES-21,1);
  box(main_options_window,0,0);

  MENU* main_options_menu;
  main_options_menu = Menu::Create_Menu(Options);
  set_menu_win(main_options_menu,main_options_window);
  set_menu_format(main_options_menu,2,3);
  set_menu_sub(main_options_menu,derwin(main_options_window,LINES - (LINES-18),COLS-4,1,1));
  post_menu(main_options_menu);
  wgetch(main_options_window);

  unpost_menu(main_options_menu);

  delwin(main_options_window);
  delwin(game_window);
  */
  
  UI ui;
  ui.GenerateUI();
  Initialize::UnInitialize();
  return 0;
}
