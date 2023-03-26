#include <Menu.h>

MENU* Menu::Create_Menu(std::vector<std::string>& Choices)
{
  MENU *my_menu;
  ITEM **items;
  items = new ITEM*[Choices.size()];
  for(int i =0;i<Choices.size();i++)
  {
    items[i] = new_item(Choices[i].c_str(),"");
  }
  items[Choices.size()] = new_item(NULL,NULL);
  my_menu=new_menu((ITEM**)items);
  return my_menu;
}
