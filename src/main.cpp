#include <iostream>
#include <locale.h>
#include <libgen.h>
#include <utf8cpp/utf8.h>
#include <nlohmann/json.hpp>
#include <ncursesw/ncurses.h>
#include "consts.hpp"
#include "functions.hpp"
#include "menu_elements.hpp"
using namespace std;
using TJson = nlohmann::ordered_json;



int main(int argc, char **argv)
{

    setlocale(LC_ALL, "");
    LOCALE = getLocale();
    std::locale::global(std::locale(LOCALE));
    std::wcout.imbue(std::locale(LOCALE));

    /**
     * @brief commands -- массив символов из входной строки
     * для выполения комманд по коротким клавишам.
     * Если переданы символы комманд,
     * то сылка на них сохраняется в переменной commands
     * и передаётся в меню для выполнения комманд
     */
    char* commands = nullptr;
    if (argc > 1)
    {
        if (argv[1][0] == '-')
        {
            paramParser(argc, argv);
            return 0;
        }
        commands = argv[1];
    }

    std::wcout << L"Читаем конфиг..." << std::endl;

    if (!fileExist(CONF_FILE_NAME))
    {
        std::wcerr << L"В текущей папке запуска конфигурационного файла не найдено." << std::endl;
        char *fullExePath = new char[EXEC_PATH_MAX];
        ssize_t resultStrLength = readlink("/proc/self/exe", fullExePath, EXEC_PATH_MAX - 1);
        if (resultStrLength == -1) {
            std::wcerr << L"Ошибка при получении пути исполняемого файла. выполнении readlink" << std::endl;
            throw 1;
        }
        fullExePath[resultStrLength] = '\0'; // Для гарантии корректного окончания строки
        char *dir = dirname(fullExePath);
        std::wcout << L"Папка расположение исполнгяемого файла: " << dir << std::endl;
        CONF_FILE_NAME = std::string(dir) + '/' + CONF_FILE_NAME;
        delete[] fullExePath;
        std::wcout << L"Полный путь файла-конфига: " << CONF_FILE_NAME.c_str() << std::endl;
        if (!fileExist(CONF_FILE_NAME))
        {
            std::wcerr << L"В папке исполняемого файла конфигурационного файла не найдено." << std::endl;
            throw 1;
        }
    }

    JSON_CONF = readConf();
    std::wcout << L"...ок." << std::endl;
    // std::wcout << "Проверяем commands..." << std::endl;
    if (!JSON_CONF.contains(CONF_COMMANDS))
    {
        std::wcerr << L"Список commands отсутствует." << std::endl;
        throw 1;
    }

    JSON_CMD_TEMPLATES = JSON_CONF[CONF_COMMANDS];

    std::wcout << L"Выводим commands..." << std::endl;
    printCmdTemplates();
    std::wcout << L"===================" << std::endl;

    if (!JSON_CONF.contains(CONF_MENU))
    {
        std::wcerr << L"Ключ " << CONF_MENU << L" отсутствует." << std::endl;
        throw 2;
    }

    std::wcout << L"Запускаем меню..." << std::endl;

    MenuTable menuTable(JSON_CONF[CONF_MENU]);
    int i = menuTable.run_menu(commands);
    std::wcout << i << std::endl;



    // setlocale(LC_ALL, "");
    // // setlocale(LC_ALL, "ru_RU.utf8");
    // initscr();
    // noecho();
    // cbreak();
    // curs_set(MY_CURSOR_HIDEN);
    // // raw();

    // if (!has_colors())
    // {
    //     return -1;
    // }

    // start_color();
    // init_pair(1, COLOR_WHITE, COLOR_BLUE);

    // box(stdscr, 0, 0);
    // refresh();

    // int yMax, xMax, height, width, start_y, start_x, padding;
    // getmaxyx(stdscr, yMax, xMax);
    // mvprintw(1, 2, "yMAX: %d, xMAX: %d", yMax, xMax);
    // mvprintw(2, 2, "Для выхода нажмите 'x'");

    // padding = min(yMax, xMax) / 4;
    // start_x = padding;
    // start_y = start_x;
    // height  = yMax - 2 * start_y;
    // width   = xMax - 2 * start_x;

    // WINDOW * win = newwin(height, width, start_y, start_x);
    // box(win, 0, 0);
    // wrefresh(win);
    // refresh();

    // const int count_menus = 4;
    // Menu menus[count_menus] = {
    //     Menu("File", 'f'),
    //     Menu("Edit", 'e'),
    //     Menu("Opt", 'o'),
    // };

    // MenuBar menubar = MenuBar(win, menus, count_menus);
    // menubar.draw();

    // char ch;
    // while (ch = wgetch(win))
    // {
    //     menubar.handleTrigger(ch);
    //     menubar.draw();
    // }

    // wgetch(win);

    // // pause
    // mvprintw(3, 2, "Конец игры. Нажмите любую кнопку для выхода");
    // getch();
    // endwin();

    return 0;
}

// https://nlohmann.github.io/json/features/object_order/#notes-on-parsing
// g++ test.cpp -lncurses -o test && ./test
// cd /home/ar/Documents/Programing/PycharmProjects/ncurses_test
// touch test.cpp && make && ./test
