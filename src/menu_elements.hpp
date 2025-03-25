#ifndef _MENU_ELEMENTS_HPP_
#define _MENU_ELEMENTS_HPP_
#include <nlohmann/json.hpp>
#include <ncursesw/ncurses.h>
#include <unistd.h>
#include "consts.hpp"
#include "functions.hpp"
#include "consts.hpp"
using namespace std;
using TJson = nlohmann::ordered_json;



struct ActionItem
{
    std::wstring title = L"";
    std::wstring hot_key_str = L"";
    wchar_t hot_key_code = (wchar_t)0;
    std::wstring command = L"";

    ActionItem() : title(L""), hot_key_str(L""), hot_key_code((wchar_t)0), command(L"") {}
    ActionItem(TJson& jsonActOne);
    std::wstring getValue(TJson& actOne, const std::string name, std::wstring log_prefix = L"");
    void fromJson(TJson& jsonActOne);
};



struct MenuHeaderItem
{
    std::wstring title = L"";
    int width = 0;
    MenuHeaderItem(std::wstring title, int width) : title(title), width(width) {}
};



using MenuHeaderRow = std::vector<MenuHeaderItem>;
using ActionsList = std::vector<ActionItem>;



struct MenuRow
{
    std::wstring itemTitle = L"";
    std::wstring itemHint = L"";
    int itemType = ITEM_TYPE_DIVIDER;
    std::wstring ip = L"";
    std::wstring user = L"";
    std::wstring pass = L"";
    ActionsList actions = {};

    MenuRow() : itemTitle(L""), itemHint(L""), itemType(ITEM_TYPE_DIVIDER), actions({}) {}
    MenuRow(TJson& jsonMenuItem, MenuHeaderRow& head);
    std::wstring toString();
};



using MenuListItems = std::vector<MenuRow>;



#define LAST_MV_NONE  (int)0
#define LAST_MV_LEFT  (int)1
#define LAST_MV_RIGHT (int)2

struct MenuTable
{
    TJson& jsonMenu;
    std::wstring title;
    int selected_item;
    int selected_action;
    MenuHeaderRow head;
    MenuListItems rows;
    int lastMoveSelector = LAST_MV_NONE;

    MenuTable(TJson& jsonMenu) : jsonMenu(jsonMenu), title(L""), selected_item(0), selected_action(0), head({}), rows({}) {}

    int getWidth(int between = 3) const;

    void selectorMoveHome();

    void selectorMoveEnd();

    void selectorMoveUp();

    void selectorMoveDown();

    void selectorMoveLeft();

    void selectorMoveRight();

    bool selectActionByKey(wchar_t& key);

    void checkAction();

    void execItem(char *commands);

    void make_header();

    bool keyInMenu(wchar_t key) const;

    void update_free_hot_keys();

    void make_table();

    void print_row(int index) const;

    std::wstring make_line(int position) const;

    void print() const;

    int run_menu(char* commands);

};



void printCmdTemplates();
std::wstring getCmdTemplateByName(std::wstring name);



#endif // MENU_ELEMENTS_HPP
