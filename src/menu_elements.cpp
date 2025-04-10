#include <iostream>
#include <nlohmann/json.hpp>
#include "menu_elements.hpp"
using TJson = nlohmann::ordered_json;
using namespace std;



ActionItem::ActionItem(TJson& jsonActOne)
{
    fromJson(jsonActOne);
}


std::wstring ActionItem::getValue(TJson& jsonActOne, const std::string name, std::wstring log_prefix)
{
    return (jsonActOne.contains(name)
                ? stringToWString(jsonActOne[name])
                : L"");
}



void ActionItem::fromJson(TJson& jsonActOne)
{
    ActionItem::title        = getValue(jsonActOne, CONF_ACT_NAME);
    ActionItem::hot_key_str  = (getValue(jsonActOne, CONF_ACT_KEY_STR).empty()
                                   ? KEY_STR_NONE
                                   : insertHighLightHotKey(getValue(jsonActOne, CONF_ACT_KEY_STR)));
    ActionItem::hot_key_code = (jsonActOne.contains(CONF_ACT_KEY_CODE) ? (wchar_t)(stringToWString(jsonActOne[CONF_ACT_KEY_CODE])[0]) : KEY_NONE);

    if (ActionItem::title != CONF_ACT_NAME_SUBMENU)
    {
        ActionItem::command = getCmdTemplateByName(getValue(jsonActOne, CONF_ACT_CMD));
        replaceSubstring(ActionItem::command, TEMPLATE_STR, getValue(jsonActOne, CONF_ACT_PARAM));
    }
}



MenuRow::MenuRow(TJson& jsonMenuItem, MenuHeaderRow& head)
{
    // строка меню, включающая название и действия
    itemTitle = (jsonMenuItem.contains(CONF_MENU_ITEM_TITLE_STR) ? stringToWString(jsonMenuItem[CONF_MENU_ITEM_TITLE_STR]) : L"") ;
    itemHint  = (jsonMenuItem.contains(CONF_MENU_ITEM_TITLE_HINT) ? stringToWString(jsonMenuItem[CONF_MENU_ITEM_TITLE_HINT]) : L"");
    itemType  = (jsonMenuItem.contains(CONF_MENU_ITEM_TYPE) ? (int)jsonMenuItem[CONF_MENU_ITEM_TYPE] : (int)ITEM_TYPE_DIVIDER);

    ip   = (jsonMenuItem.contains(CONF_MENU_ITEM_IP)   ? stringToWString(jsonMenuItem[CONF_MENU_ITEM_IP])   : L"") ;
    user = (jsonMenuItem.contains(CONF_MENU_ITEM_USER) ? stringToWString(jsonMenuItem[CONF_MENU_ITEM_USER]) : L"") ;
    pass = (jsonMenuItem.contains(CONF_MENU_ITEM_PASS) ? stringToWString(jsonMenuItem[CONF_MENU_ITEM_PASS]) : L"") ;

    actions.resize(head.size() - 1);

    if (((int)itemType & ITEM_TYPE_ACTIVES) > 0)
    {
        if (!jsonMenuItem.contains(CONF_MENU_ITEM_ACTIONS)) {
            std::wcerr << L"Menu Item: [" << itemTitle << L"] отсутствуют тействия [" << CONF_MENU_ITEM_ACTIONS << L"]" << std::endl;
            throw 1;
        }
        TJson& jsonActions = jsonMenuItem[CONF_MENU_ITEM_ACTIONS];
        for (int actIndex = 0; actIndex < jsonActions.size(); ++actIndex)
        {
            ActionItem actionItem(jsonActions[actIndex]);
            if (!ip.empty())   { replaceSubstring(actionItem.command, TEMPLATE_IP,   ip);   }
            if (!user.empty()) { replaceSubstring(actionItem.command, TEMPLATE_USER, user); }
            if (!pass.empty()) { replaceSubstring(actionItem.command, TEMPLATE_PASS, pass); }

            // индекс этого действия в head
            size_t indexOnActions = -1;
            // найти это действие в head
            for (int headIndex = 0; headIndex < head.size(); ++headIndex)
            {
                if (actionItem.title == head[headIndex].title)
                {
                    if (head[headIndex].width < termColors.clearStr(actionItem.hot_key_str).length())
                    {
                        head[headIndex].width = termColors.clearStr(actionItem.hot_key_str).length();
                    }
                    indexOnActions = headIndex - 1;
                    break;
                }
            }

            // Индекс элемента, который нужно заменить
            auto it = actions.begin() + indexOnActions;
            // Удаление элемента по итератору
            actions.erase(it);
            // Вставка нового элемента на то же место
            actions.insert(it, actionItem);
        }
    }
}



std::wstring MenuRow::toString()
{
    return  L"tltle: \""   + itemTitle                       + L"\",\n" +
            L"hint: \""    + itemHint                        + L"\",\n" +
            L"type: \""    + std::to_wstring(itemType)       + L"\",\n" +
            L"actions: \"" + std::to_wstring(actions.size()) + L"\";";
}



int MenuTable::getWidth(int between) const {
    int width = 0;
    for (int i = 0; i < head.size(); i++) { width += (head[i].width + between); }
    return width - between;
}



void MenuTable::selectorMoveHome()
{
    selected_item = 0;
    checkAction();
}



void MenuTable::selectorMoveEnd()
{
    selected_item = rows.size() - 1;
    checkAction();
}



void MenuTable::selectorMoveUp()
{
    if (selected_item > 0) { selected_item -= 1; }
    else { selected_item = rows.size() - 1; }
    if (rows[selected_item].itemType == ITEM_TYPE_DIVIDER) { selectorMoveUp(); }
    checkAction();
}



void MenuTable::selectorMoveDown()
{
    if (selected_item < (rows.size() - 1)) { selected_item += 1; }
    else { selected_item = 0; }
    if (rows[selected_item].itemType == ITEM_TYPE_DIVIDER) { selectorMoveDown(); }
    checkAction();
}



void MenuTable::selectorMoveLeft()
{
    lastMoveSelector = LAST_MV_LEFT;
    if (selected_action > 0) { selected_action -= 1; }
    else { selected_action = rows[selected_item].actions.size() - 1; }
    checkAction();
};



void MenuTable::selectorMoveRight()
{
    lastMoveSelector = LAST_MV_RIGHT;
    if (selected_action < (rows[selected_item].actions.size() - 1)) { selected_action += 1; }
    else { selected_action = 0; }
    checkAction();
};



bool MenuTable::selectActionByKey(wchar_t& key)
{
    for (int indexRow = 0; indexRow < rows.size(); ++indexRow) {
        MenuRow& row = rows[indexRow];
        ActionsList& actList = row.actions;
        for (int indexAct = 0; indexAct < actList.size(); ++indexAct) {
            if (actList[indexAct].hot_key_code == key)
            {
                selected_item = indexRow;
                selected_action = indexAct;
                return true;
            }
        }
    }
    return false;
}



void MenuTable::checkAction()
{
    // Ok если
    // Итем это субменю И актИндекс = 0
    // ИЛИ
    // актИндекс > 0 И Комманда есть
    bool ok = ((rows[selected_item].itemType == ITEM_TYPE_SUBMENU) && (selected_action == 0)) ||
              ((selected_action > 0) && !rows[selected_item].actions[selected_action].command.empty());
    if (ok)
    {
        lastMoveSelector = LAST_MV_NONE;
    }
    else
    {
        switch (lastMoveSelector) {
        case LAST_MV_LEFT:
            selectorMoveLeft();
            break;
        case LAST_MV_RIGHT:
        default:
            selectorMoveRight();
            break;
        }
    }
}



void MenuTable::execItem(char* commands)
{
    if (rows[selected_item].itemType == ITEM_TYPE_SUBMENU)
    {
        MenuTable menu(jsonMenu[CONF_MENU_ITEMS][selected_item][CONF_MENU]);
        menu.run_menu(commands);
    }
    else
    {

        // execSysCommand(rows[selected_item].actions[selected_action].command);
        // return;

        pid_t pid = fork(); // Создаем дочерний процесс
        if (pid < 0) {
            std::wcerr << L"Ошибка при создании процесса | Fork failed\n";
            return;
        } else if (pid == 0) {
            // Это дочерний процесс
            // std::wcout << L"Это дочерний процесс | I'm the child process with PID: " << getpid() << L"\n";
            // Запускаем внешнюю программу
            execSysCommand(rows[selected_item].actions[selected_action].command);
            return;
        } else {
            // Это родительский процесс
            // std::wcout << L"Это родительский процесс | I'm the parent process with PID: " << getpid() << L"\n";
            // int status;
            // // Ждем завершения дочернего процесса
            // waitpid(pid, &status, 0);
            // if (WIFEXITED(status)) {
            //     std::cout << "Child exited with status " << WEXITSTATUS(status) << "\n";
            // }
        }
    }
}



void MenuTable::make_header()
{
    std::wstring title = (jsonMenu.contains(CONF_MENU_TITLE) ? stringToWString(jsonMenu[CONF_MENU_TITLE]) : L"----");
    head.insert(head.begin() + 0, MenuHeaderItem(title, max((int)title.length(), (int)TABLE_TITLE_WIDTH_MIN)));
    head.insert(head.begin() + 1, MenuHeaderItem(CONF_ACT_NAME_SUBMENU,  TABLE_SUBMENU_WIDTH_MIN));

    TJson& jsonMenuItems = jsonMenu[CONF_MENU_ITEMS];

    for (int itemIndex = 0; itemIndex < jsonMenuItems.size(); ++itemIndex)
    {
        // текущий трассируемый элемент меню меню
        TJson& jsonThisItem = jsonMenuItems[itemIndex];
        if (((int)jsonThisItem[CONF_MENU_ITEM_TYPE] & ITEM_TYPE_ACTIVES) > 0)
        {
            // Обновление значения ширины колонки itemTitle
            std::wstring itemTitleStr = stringToWString(jsonThisItem[CONF_MENU_ITEM_TITLE_STR]);

            // Обновление ширины поля заголовка
            if (head[0].width < (int)itemTitleStr.length()) { head[0].width = (int)itemTitleStr.length(); }

            // Действия
            TJson& actions = jsonThisItem[CONF_MENU_ITEM_ACTIONS];
            for (int actIndex = 0; actIndex < actions.size(); ++actIndex)
            {
                // текущее трассируемое действие
                TJson& jsonActOne = actions[actIndex];
                if (!jsonActOne.contains(CONF_ACT_NAME)) {
                    // если нет названия действия, то это ошибка
                    std::wcerr << L"ERROR. MENU: [" << itemTitleStr << L"]. Поля имени действия [" << CONF_ACT_NAME << "] -- нет." << std::endl;
                    throw 1;
                }

                std::wstring actOneName = stringToWString(jsonActOne[CONF_ACT_NAME]);

                if (!jsonActOne.contains(CONF_ACT_KEY_STR)) {
                    std::wcerr << L"ERROR. MENU: [" << itemTitleStr << L"].ACT ONE NAME: [" << actOneName << L"]. ";
                    std::wcerr << L"поля [KEY_STR] -- нет." << std::endl;
                    throw 1;
                }

                std::wstring actOneKeyStr = stringToWString(jsonActOne[CONF_ACT_KEY_STR]);

                bool found = false; // найдено ли название действия в массиве заголовков
                for (int headIndex = 0; headIndex < head.size(); ++headIndex)
                {
                    if (actOneName == head[headIndex].title)
                    {
                        // Если действие есть, то просо проверить длину текста
                        if (head[headIndex].width < actOneKeyStr.length()) { head[headIndex].width = actOneKeyStr.length(); }
                        found = true;
                        break;
                    }
                }

                // Если действие НЕ найдено, то просто добавляем
                if (!found)
                {
                    head.push_back({actOneName, max((int)actOneName.length(), TABLE_ACTION_WIDTH_MIN)}); // (int)utf8_length(actName)
                }
            }
        }
    }
}



bool MenuTable::keyInMenu(wchar_t key) const
{
    for (int rowIndex = 0; rowIndex < rows.size(); ++rowIndex)
    {
        const MenuRow& row = rows[rowIndex];
        const ActionsList& actions = row.actions;
        for (int actIndex = 0; actIndex < actions.size(); ++actIndex)
        {
            const ActionItem& act = actions[actIndex];
            if (act.hot_key_code == key)
            {
                return true;
            }
        }
    }
    return false;
}



void MenuTable::update_free_hot_keys()
{
    int     hotKeyListSize = sizeof(HOT_KEYS_LIST) / sizeof(wchar_t) - 1;

    for (int rowIndex = 0; rowIndex < rows.size(); ++rowIndex)
    {
        MenuRow& row = rows[rowIndex];
        // std::wcout << L"item: " << row.itemTitle << std::endl; // !!!
        if ((row.itemType & ITEM_TYPE_ACTIVES) > 0)
        {
            ActionsList& actions = row.actions;
            for (int actIndex = 0; actIndex < actions.size(); ++actIndex) {
                ActionItem& actOne = actions[actIndex];
                if (!actOne.command.empty() || (actOne.command.empty() && (row.itemType == ITEM_TYPE_SUBMENU)))
                {
                    // std::wcout << L"1 act:[" << actOne.title << L"] str:[" << actOne.hot_key_str << "]" << L"key:[" << actOne.hot_key_code << "]"   << L"[" << actOne.command << "]"  << std::endl; // !!!
                    if (actOne.hot_key_code == KEY_NONE)
                    {
                        // действие без хот_кей
                        // actOne.hot_key_str = KEY_STR_NONE;
                        // перебираем hotKeyList и ищем те, что не используются в меню
                        for (int indexHotKey = 0; indexHotKey < hotKeyListSize; ++indexHotKey) {
                            // если хоткей не используется, то применяем его
                            if (!keyInMenu(HOT_KEYS_LIST[indexHotKey]))
                            {
                                actOne.hot_key_code = HOT_KEYS_LIST[indexHotKey];
                                actOne.hot_key_str = termColors.blue + HOT_KEYS_LIST[indexHotKey] + termColors.off;
                                break;
                            }
                        }
                    }
                    // std::wcout << L"2 act:[" << actOne.title << L"] str:[" << actOne.hot_key_str << "]" << L"key:[" << actOne.hot_key_code << "]"   << L"[" << actOne.command << "]"  << std::endl; // !!!
                }
                // Если это подменю, то других комманд не должно быть
                if (row.itemType == ITEM_TYPE_SUBMENU) { break; }
            }
        }
    }
}



void MenuTable::make_table()
{
    // Название меню в целом
    title         = (jsonMenu.contains(CONF_MENU_TITLE) ? stringToWString(jsonMenu[CONF_MENU_TITLE]) : L"");

    // перебираем элементы меню
    TJson& jsonMenuItems = jsonMenu[CONF_MENU_ITEMS];
    for (int itemIndex = 0; itemIndex < jsonMenuItems.size(); ++itemIndex)
    {
        MenuRow row(jsonMenuItems[itemIndex], head);
        rows.push_back(row);
    }

    update_free_hot_keys();

    // пункт меню по умолчанию
    selected_item  = (jsonMenu.contains(CONF_MENU_DEF_ITEM_NAME) ? (int)jsonMenu[CONF_MENU_DEF_ITEM_NAME] : CONF_MENU_DEF_ITEM_VALUE);
    if (selected_item < 0)                  { selected_item = rows.size() + selected_item; }
    if (selected_item > (rows.size() - 1))  { selected_item = rows.size() - 1; }
}



void MenuTable::print_row(int index) const
{
    const MenuRow& row = rows[index];
    bool rowSelected = (selected_item == index);
    bool actSelected = false;
    if (row.itemType != ITEM_TYPE_DIVIDER)
    {
        // Колонка с номером строки и указателем выбранной строки
        std::wcout << TBL_CH_LF_LF << L" "
                   << (rowSelected ? termColors.green : termColors.dark_gray)
                   << format_str(std::to_wstring(index), 3, '0', FORMAT_STR_RIGHT)
                   << (rowSelected ? termColors.off : termColors.off)
                   << (rowSelected ? L" > " : L"   ")
                   // Название меню
                   << TBL_CH_CN__I << L" "
                   << format_str(row.itemTitle, head[0].width, L'.', FORMAT_STR_LEFT)
                   << (rowSelected ? termColors.off : L"");
        // Действия
        for (int actIndex = 0; actIndex < row.actions.size(); actIndex++)
        {
            std::wstring actHotKeyStr =
                ((row.actions[actIndex].hot_key_str.empty() || (row.actions[actIndex].hot_key_str == KEY_STR_NONE))
                     ? format_str(
                            row.actions[actIndex].hot_key_str, head[actIndex+1].width, L' ', FORMAT_STR_CENTR)
                     : format_str(
                            (hasHighLighted(row.actions[actIndex].hot_key_str)
                                ? row.actions[actIndex].hot_key_str
                                : termColors.underline + row.actions[actIndex].hot_key_str + termColors.off
                            ),
                            head[actIndex+1].width,
                            L' ',
                            FORMAT_STR_CENTR)
                 );
            actSelected = (rowSelected && (selected_action == actIndex));
            std::wcout << " " << TBL_CH_CN__I << L" "
                       << (actSelected
                               ? termColors.selected + termColors.clearStr(actHotKeyStr) + termColors.off
                               : actHotKeyStr
                           );
        }
        std::wcout << L' ' << TBL_CH_RT_RT << std::endl;
    }
    else
    {
        std::wcout << make_line(TBL_LINE_CN) << std::endl;
    }
}



std::wstring MenuTable::make_line(int position) const
{
    std::wstringstream ss;
    switch (position)
    {
    case TBL_LINE_UP:

        ss << TBL_CH_UP_LF << std::setfill(TBL_CH_UP_UP) << std::setw(7) << L"" << std::setfill(L' ') << TBL_CH_UP_CN << TBL_CH_UP_UP;
        for (int i = 0; i < head.size(); i++)
        {
            ss << std::setfill(TBL_CH_UP_UP) << std::setw(head[i].width) << L"" << std::setfill(L' ');
            if (i < (head.size()-1)) { ss << TBL_CH_UP_UP << TBL_CH_UP_CN << TBL_CH_UP_UP; }
        }
        ss << TBL_CH_UP_UP << TBL_CH_UP_RT;
        break;

    case TBL_LINE_CN:
        ss << TBL_CH_LF_CN << std::setfill(TBL_CH_CN_CN) << std::setw(7) << L"" << std::setfill(L' ') << TBL_CH_CN_CR << TBL_CH_CN_CN;
        for (int i = 0; i < head.size(); i++)
        {
            ss << std::setfill(TBL_CH_CN_CN) << std::setw(head[i].width) << L"" << std::setfill(L' ');
            if (i < (head.size()-1)) { ss << TBL_CH_CN_CN << TBL_CH_CN_CR << TBL_CH_CN_CN; }
        }
        ss << TBL_CH_CN_CN << TBL_CH_RT_CN;
        break;

    case TBL_LINE_DN:
        ss << TBL_CH_DN_LF << std::setfill(TBL_CH_DN_DN) << std::setw(7) << L"" << std::setfill(L' ') << TBL_CH_DN_CN << TBL_CH_DN_DN;
        for (int i = 0; i < head.size(); i++)
        {
            ss << std::setfill(TBL_CH_DN_DN) << std::setw(head[i].width) << L"" << std::setfill(L' ');
            if (i < (head.size()-1)) { ss << TBL_CH_DN_DN << TBL_CH_DN_CN << TBL_CH_DN_DN; }
        }
        ss << TBL_CH_DN_DN << TBL_CH_DN_RT;
        break;

    default:
        std::wcerr << L"Не верный тип меню" << std::endl;
        throw 1;
        break;
    }
    return ss.str();
}



void MenuTable::print() const
{
    clearScreen();

    // std::wcout
    //     // << L"Rows [" << rows.size()
    //     // << L" | Row: [" << selected_item << L"]" << L" | Act: [" << selected_action << L"]"
    //     // << L" | type: " << rows[selected_item].itemType
    //     << L" | cmd: [" << rows[selected_item].actions[selected_action].command << L"]"
    //     << L" | " << std::endl;

    std::wcout << make_line(TBL_LINE_UP) << std::endl;

    // Начало таблицы
    std::wcout << TBL_CH_LF_LF << L" No    ";
    for (int i = 0; i < head.size(); i++)
    {
        std::wstringstream wss;
        if (i == 0) { wss << std::left << std::setw(head[i].width) << head[i].title.c_str(); }
        else        { wss << std::right << format_str(head[i].title, head[i].width, L' ', FORMAT_STR_CENTR); }
        std::wcout << TBL_CH_CN__I << L" " << wss.str() << L" ";
    }
    std::wcout << TBL_CH_RT_RT << std::endl;

    // строка разделитель
    std::wcout << make_line(TBL_LINE_CN) << std::endl;
    // строка для показа хинта текущего элемента внизу таблицы
    std::wstring hint = L"";
    // строки меню
    for (int i = 0; i < rows.size(); i++)
    {
        print_row(i);
        if (selected_item == i) { hint = rows[i].itemHint; }
    }
    std::wcout << make_line(TBL_LINE_CN) << std::endl;
    std::wcout << TBL_CH_LF_LF << L"       " << TBL_CH_CN__I << L" " << format_str(hint, getWidth(), L' ', FORMAT_STR_LEFT) << L" " << TBL_CH_RT_RT << std::endl;
    std::wcout << TBL_CH_LF_LF << L"       " << TBL_CH_CN__I << L" " << format_str(insertHighLightHotKey(L"[~|Esc] Выход | &Edit"), getWidth(), L' ', FORMAT_STR_LEFT) << L" " << TBL_CH_RT_RT << std::endl;
    std::wcout << make_line(TBL_LINE_DN) << std::endl;
};



int MenuTable::run_menu(char* commands)
{
    /**
     * @brief command_key
     * Если в параметре передана строка с последовательностью комманд,
     * то берём первую комманду и запускаем меню.
     */
    wchar_t command_key = KEY_NONE;
    if (!is_null(commands) && commands[0] != '\0')
    {
        std::wcout << L"cmd:[" << commands[0] << L"]" << std::endl;
        command_key = commands[0];
        commands++;
    }

    if (jsonMenu.contains(CONF_MENU_ITEMS))
    {
        // Сборка верхней строки меню с названием меню и списком действий
        make_header();
        // Сборка полной таблицы
        make_table();
        // Установка горизонтаьлного селектора на фактическое действие
        checkAction();

        std::wstring cmdEdit;
        wchar_t key;
        do
        {
            print();
            std::wcout << L"> ";

            if (command_key == KEY_NONE) { key = my_getch();  }
            else                         { key = command_key; }
            command_key = KEY_NONE;

            std::wcout << L"Key: [" << (long)key << L"]" << std::endl;
            switch (key) {
            case (wchar_t)27:
                key = my_getch();
                std::wcout << L"Key: [" << (long)key << L"]" << std::endl;
                switch (key) {
                case (wchar_t)27:
                    key = KEY_QUIT;
                    break;
                case (wchar_t)91:
                    key = my_getch();
                    std::wcout << L"Key: [" << (long)key << L"]" << std::endl;
                    switch (key) {
                    case (wchar_t)72: // Home
                        selectorMoveHome();
                        break;
                    case (wchar_t)70: // End
                        selectorMoveEnd();
                        break;
                    case (wchar_t)65: // up
                        selectorMoveUp();
                        break;
                    case (wchar_t)66: // dn
                        selectorMoveDown();
                        break;
                    case (wchar_t)67: // Right
                        selectorMoveRight();
                        break;
                    case (wchar_t)68: // Left
                        selectorMoveLeft();
                        break;
                    default:
                        break;
                    }
                    break;

                default:
                    break;
                }
                break;

            case (wchar_t)10:
                execItem(commands);
                break;
            case KEY_EDIT:
                std::wcout << L"Редактирование конфиг-файла: [" << CONF_FILE_NAME.c_str() << "]" << std::endl;
                cmdEdit = getCmdTemplateByName(L"%edit%");
                replaceSubstring(cmdEdit, TEMPLATE_STR, stringToWString(CONF_FILE_NAME));
                std::wcout << L"Редактирование конфиг-файла: [" << cmdEdit << "]" << std::endl;
                execSysCommand(cmdEdit);
                break;

            default:
                if (selectActionByKey(key))
                {
                    print();
                    execItem(commands);
                }
                break;
            }

        } while (key != KEY_QUIT);

    }
    else
    {
        std::wcout << L"В меню [" << title << L"] нет пунктов меню." << std::endl;
        throw 1;
    }
    return 0;
};



void printCmdTemplates()
{
    for (int i = 0; i < JSON_CMD_TEMPLATES.size(); ++i) {
        std::wcout
            << L"id: " <<  i
            << L" | name: "
            << stringToWString(JSON_CMD_TEMPLATES[i][CONF_COMMAND_NAME])
            << L" | value: "
            << stringToWString(JSON_CMD_TEMPLATES[i][CONF_COMMAND_VALUE])
            << std::endl;
    }
}


std::wstring getCmdTemplateByName(std::wstring cmdName)
{
    for (int i = 0; i < JSON_CMD_TEMPLATES.size(); ++i) {
        if (stringToWString(JSON_CMD_TEMPLATES[i][CONF_COMMAND_NAME]).compare(cmdName) == 0)
        {
            return stringToWString(JSON_CMD_TEMPLATES[i][CONF_COMMAND_VALUE]);
        }
    }
    return cmdName;
}
