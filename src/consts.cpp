#ifndef _CONSTS_CPP_
#define _CONSTS_CPP_
#include <nlohmann/json.hpp>
#include "consts.hpp"
using TJson = nlohmann::ordered_json;



std::string LOCALE = "";



const std::wstring getVersion()
{
    return  L"SM - Simple Menu, версия " + std::to_wstring(VERSION_MAJOR) + L"." + std::to_wstring(VERSION_MINOR) + L"." + std::to_wstring(VERSION_UPDATE) + L"-release " + VERSION_DATE + L" (GitHub-Revision)\n"
            L"© RI-Network Technical Union. 2025\n"
            L"\n"
            L"Программа написана на C++ \n"
            L"с использованием библиотеки nlohmann/json.hpp \n"
            L"Предполагалась для использования библитека ncurses, но пока без неё. \n"
            L"Реализация с помощью ncurses запланирована на следующий выпуск. \n"
            L"\n"
            L"Программу можно использовать \"как есть\". \n"
            L"Никакая гарантия ни на что не предоставляется в пределах, допускаемых законом. \n\n";
}



const std::wstring getHelp()
{
    std::wstring s = L"Начало строки " + std::wstring(1, KEY_EDIT) + L"конец строки";
    return L"Использование программы:\n"
           L"  sm [...]\n"
           L"  --help, -h       : показать справку\n"
           L"  --version, -v    : показать версию программы\n"
           L"  --locale, -l     : Показать используемую локаль\n"
           L"  " + std::wstring(1, KEY_EDIT) + L"                : Редактировать файл конфигурации с помощью внешней программы\n"
           L"  <a...z0...9>     : Последовательность коротких кнопок для выполения комманд\n"
           L"                     Перечисленные символы соответсвуют последователному нажатию кнопок\n"
           L"                     для выполнения нужного пункта меню\n"
           L"  Имя файла конфигурации меню \"sm.conf.json\". \n"
           L"      Должен находиться в текущей папаке, из которой производится запуск,\n"
           L"      или в папаке, где находится исполняемый файл.\n"
           L"      json-файл поддерживает комментарии вида:\n"
           L"      // Комментарий до конца строки\n"
           L"      /* Комментарий внутри строки */\n"
           L"      При сохранении файла конфигурации из программы коментарии не сохраняются.\n"
           L"\n"
           L"Управление:\n"
           L"  Для перемещения по пунктам меню можно использовать:\n"
           L"                          Клавиши управления курсором,\n"
           L"                          Home, End\n"
           L"  Выполнить пункт меню:   Enter\n"
           L"  Для мгновенного выполнения комманды\n"
           L"      можно использоваь короткую клавишу (hot_key),\n"
           L"      символ которой задан в конфиге меню, в параметре \"hot_key_code\": \"с\"\n"
           L"      и её подсказка в параметре \"hot_key_str\": \"строка\"\n"
           L"      для подсветки символа используется \"&\", например: \"hot_key_str\": \"st&c\"\n"
           L"\n"
           L"  Для редактирования файла конфигурации используется hot-key \"" + std::wstring(1, KEY_EDIT) + L"\"\n"
           L"  Для возврата в верхнее меню и для выхода из программы используйте hot-key \"" + std::wstring(1, KEY_QUIT) + L"\" (~ или Esc)\n"
           L"";
}



// имя конфиг-файла с данными в json формате
std::string CONF_FILE_NAME = "sm.conf.json";



TJson JSON_CONF = nullptr;
TJson JSON_CMD_TEMPLATES = nullptr;



#endif
