#ifndef _CONSTS_HPP_
#define _CONSTS_HPP_
#include <regex>
#include <string>
#include <nlohmann/json.hpp>
using TJson = nlohmann::ordered_json;



/**
 * @brief LOCALE -- Строка, содержащая имя локали для вывода данных в терминал, используемая в приложении.
 * Локаль ищется из системных ru*utf8
 */
extern std::string LOCALE;

const int VERSION_MAJOR         = 0;
const int VERSION_MINOR         = 1;
const int VERSION_UPDATE        = 2;
const std::wstring VERSION_DATE = L"2025-03-25";

extern const std::wstring getVersion();

extern const std::wstring getHelp();

#define EXEC_PATH_MAX (int)250

/**
 *  Процедура curs_set устанавливает состояние курсора в невидимое,
 *  нормальное или очень видимое для видимости, равной 0, 1 или 2 соответственно.
 *  Если терминал поддерживает запрошенную видимость, возвращается предыдущее состояние курсора;
 *  в противном случае возвращается ошибка.
 */
#define MY_CURSOR_HIDEN  (int)0 /* курсор в невидимое     */
#define MY_CURSOR_NORM   (int)1 /* курсор в нормальное    */
#define MY_CURSOR_STRONG (int)2 /* курсор в очень видимое */

#define MY_KEY_ENTER (int)10    /* enter key */


// имя конфиг-файла с данными в json формате
extern std::string  CONF_FILE_NAME;
extern TJson        JSON_CONF;
extern TJson        JSON_CMD_TEMPLATES;


#define TEMPLATE_STR                L"%S%"
#define TEMPLATE_IP                 L"%IP%"
#define TEMPLATE_USER               L"%U%"
#define TEMPLATE_PASS               L"%P%"

// Именя полей в json-файле
// json Имя поля Списка системных комманд
#define CONF_COMMANDS               "commands"
// json Шаблоны системных комманд
#define CONF_COMMAND_NAME           "name"
#define CONF_COMMAND_VALUE          "value"

// json Поля описания меню
#define CONF_MENU                   "menu"
#define CONF_MENU_TITLE             "title"
#define CONF_MENU_DEF_ITEM_NAME     "default_item"
#define CONF_MENU_DEF_ITEM_VALUE    (int)-1
#define CONF_MENU_ITEMS             "items"

// json Поля массива пунктов меню
#define CONF_MENU_ITEM_TITLE_STR    "title_str"
#define CONF_MENU_ITEM_TITLE_HINT   "title_hint"
#define CONF_MENU_ITEM_TYPE         "type"
#define CONF_MENU_ITEM_IP           "ip"
#define CONF_MENU_ITEM_USER         "user"
#define CONF_MENU_ITEM_PASS         "pass"


#define CONF_MENU_ITEM_ACTIONS      "actions"

// json Поля массива действий
#define CONF_ACT_NAME               "name"
#define CONF_ACT_NAME_SUBMENU       L">>>"
#define CONF_ACT_KEY_STR            "hot_key_str"
#define CONF_ACT_KEY_CODE           "hot_key_code"
#define CONF_ACT_CMD                "cmd"
#define CONF_ACT_PARAM              "param"



// json Типы пунктов меню
/// @brief Элемент меню, означаюй подменю
#define ITEM_TYPE_SUBMENU           (int)0b00000001  // (int)1
/// @brief Элемент меню, означаюй системную сомманду
#define ITEM_TYPE_COMMAND           (int)0b00000010  // (int)2
/// @brief Элемент меню, означаюй "активный пункт" -- подменю или комманду
#define ITEM_TYPE_ACTIVES           (int)(ITEM_TYPE_COMMAND | ITEM_TYPE_SUBMENU) // (int)3
/// @brief Элемент меню, означаюй разделитель
#define ITEM_TYPE_DIVIDER           (int)0b00000100  // (int)4



/// @brief Верхняя рамка таблицы
#define TBL_LINE_UP (int)1
/// @brief Верхняя линия таблицы
#define TBL_LINE_CN (int)2
/// @brief Нижняя рамка таблицы
#define TBL_LINE_DN (int)3



/// @brief Выравнивание влево при форматировании строки
#define FORMAT_STR_LEFT  (int)1
/// @brief Выравнивание вправо при форматировании строки
#define FORMAT_STR_RIGHT (int)2
/// @brief Выравнивание по центру при форматировании строки
#define FORMAT_STR_CENTR (int)3



// # Table borders
// # ╔═╤═╗
// # ║ │ ║
// # ╟─┼─╢
// # ║ │ ║
// # ╚═╧═╝
// #

/*
    Шаблон выводимого меню
    --MenuTitle---------------------------------------------------------- MenuTable.title
    | 0          | 1        | 2        | 3        | 4        | 5        |
    | title      | submenus | Act1     | Act2     | Act3     | ...      | MenuTable.head.title
    | <length>   | <length> | <length> | <length> | <length> | <length> | MenuTable.head.length
    ---------------------------------------------------------------------

      title_str    0          1          2          3          4
    .-----------. .---------.----------.----------.----------.----------.
    | Menu1     | |     >>> |          |          |          |          | MenuTable.rows
    +-----------| |---------|----------|----------|----------|----------|
    | Menu2     | |         |          |          |          |          |
    '-----------' '---------'----------'----------'----------'----------'
*/

//  ╔═════╤═══════ TITLE ═════════╤═ winbox ═╤═ WEB ═╤═ zabbix ═╗
//  ║ 000. RI_MAIN 00 Bilink..... │    00    │   01  │    0z    ║
//  ║ 008. RY2 UNDER WIFI 1...... │   u21    │  uw21 │          ║
//  ║ 009. RY2 UNDER WIFI 2...... │   u21    │  uw21 │          ║
//  ╚═══════════════════════════ E-редактировать | Esc/Q-выход ═╝

// ╭─┬─╮ ┌─┬┐
// │ │ │ │┈││
// ├─┼─┤ │┄││
//        ╌
// │ │ │ ├─┼┤
// ╰─┴─╯ └─┴┘

#define TBL_CH_UP_LF  L'╔'   //  '.' (wchar_t)'╔'
#define TBL_CH_UP_UP  L'═'   //  '-' (wchar_t)'═'
#define TBL_CH_UP_CN  L'╤'   //  '.' (wchar_t)'╤'
#define TBL_CH_UP_RT  L'╗'   //  '.' (wchar_t)'╗'
#define TBL_CH_LF_LF  L'║'   //  '|' (wchar_t)'║'
#define TBL_CH_LF_CN  L'╟'   //  '+' (wchar_t)'╟'
#define TBL_CH_CN_CN  L'╌'   //  '-' (wchar_t)'─'
#define TBL_CH_CN__I  L'│'   //  '|' (wchar_t)'│'
#define TBL_CH_CN_CR  L'┼'   //  '+' (wchar_t)'┼'
#define TBL_CH_RT_RT  L'║'   //  '|' (wchar_t)'║'
#define TBL_CH_RT_CN  L'╢'   //  '+' (wchar_t)'╢'
#define TBL_CH_DN_LF  L'╚'   //  '\' (wchar_t)'╚'
#define TBL_CH_DN_DN  L'═'   //  '-' (wchar_t)'═'
#define TBL_CH_DN_CN  L'╧'   // '\'' (wchar_t)'╧'
#define TBL_CH_DN_RT  L'╝'   // '\'' (wchar_t)'╝'

// добавление отступа к отображению title и action
#define PADDING_LEFT  (int)1
#define PADDING_RIGHT (int)1
#define PADDING_TOP   (int)0
#define PADDING_BOT   (int)0

#define TABLE_TITLE_WIDTH_MIN   15 // Минимальная ширина колонки Название
#define TABLE_SUBMENU_WIDTH_MIN  5 // Минимальная ширина колонки подменю
#define TABLE_ACTION_WIDTH_MIN   7 // Минимальная ширина колонки подменю



// struct TermColorItem
// {
//     std::wstring on;
//     std::wstring off;
//     // TermColorItem(std::string on, std::string off) : on(on), off(off) {}
// };


/*
    Цвета текста:
        30 - черный
        31 - красный
        32 - зеленый
        33 - желтый
        34 - синий
        35 - пурпурный
        36 - циан
        37 - белый
    Цвета фона:
        40 - черный
        41 - красный
        42 - зеленый
        43 - желтый
        44 - синий
        45 - пурпурный
        46 - циан
        47 - белый

    echo -e "\e[31;47mТекст красного цвета на белом фоне\e[0m"
    echo -e "\e[7mТекст с инвертированными цветами\e[0m"
 */

struct TermColors
{
    const std::wstring off        = L"\x1b[0m";
    const std::wstring bold       = L"\x1b[1m";
    const std::wstring red        = L"\x1b[31m";
    const std::wstring green      = L"\x1b[32m";
    const std::wstring yellow     = L"\x1b[33m";
    const std::wstring blue       = L"\x1b[34m";
    const std::wstring magenta    = L"\x1b[35m";
    const std::wstring cyan       = L"\x1b[36m";
    const std::wstring light_gray = L"\x1b[37m";
    const std::wstring underline  = L"\x1b[4m";
    const std::wstring dark_gray  = L"\x1b[1;30m";
    const std::wstring invert     = L"\x1b[7m";
    const std::wstring selected   = L"\x1b[37;44m";
    const std::wstring pattern    = L"\\x1b\\[.*?m";

    std::wstring clearStr(std::wstring coloredStr) const
    {
        std::wregex regex(pattern);
        return std::regex_replace(coloredStr, regex, L"");
    }

};

const TermColors termColors;



const wchar_t HOT_KEYS_LIST[] = L"abcdfghijklmnoprtuvwxyz0123456789"; // eqs
#define KEY_QUIT             (wchar_t)'`'
#define KEY_NONE             (wchar_t)0
#define KEY_EDIT             (wchar_t)'e'
#define KEY_STR_NONE         termColors.dark_gray + L"." + termColors.off
#define CHAR_HIGHLIGHT       (wchar_t)'&'
#define COLOR_HIGHLIGHT      termColors.underline + termColors.yellow


#endif
