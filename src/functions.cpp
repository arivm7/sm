#ifndef _FUNCTIONS_CPP_
#define _FUNCTIONS_CPP_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <codecvt>
#include <unistd.h>
#include <ncursesw/ncurses.h>
#include <nlohmann/json.hpp>
#include "consts.hpp"
#include "functions.hpp"
using TJson = nlohmann::ordered_json;
using TVectorWString = std::vector<std::wstring>;



size_t utf8_length(const std::string& str) {
    size_t length = 0;
    for (size_t i = 0; i < str.size(); ) {
        unsigned char c = str[i];
        if (c < 0x80) {
            // 1 байт (ASCII)
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2 байта
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3 байта
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4 байта
            i += 4;
        } else {
            // Неверный байт, можно обработать ошибку
            i += 1; // или можно выбросить исключение
        }
        length++;
    }
    return length;
}



bool fileExist(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}



bool fileRemove(const std::string& filename)
{
    if (std::remove(filename.c_str()) == 0) {
        std::wcout << L"Файл успешно удален." << std::endl;
        return true;
    } else {
        std::wcerr << L"Ошибка при удалении файла." << std::endl;
        throw std::runtime_error("Ошибка при удалении файла.");
    }
}



bool fileRename(const char* oldName, const char* newName)
{
    if (std::rename(oldName, newName) == 0) {
        std::wcout << L"Файл успешно переименован." << std::endl;
        return true;
    } else {
        std::wcerr << L"Ошибка при переименовании файла." << std::endl;
        throw std::runtime_error("Ошибка при переименовании файла.");
    }
}



bool saveConf(TJson& jsonData, std::string fileName)
{
    std::string oldFileName = fileName + ".old";

    if (fileExist(oldFileName)) { fileRemove(oldFileName); }
    fileRename(fileName.c_str(), oldFileName.c_str());

    // Запись измененного JSON в файл
    std::ofstream outputFile(fileName);
    if (!outputFile.is_open()) {
        std::wcerr << L"Не удалось открыть файл для записи." << std::endl;
        std::wcout << L"Ошибка записи output.json." << std::endl;
        return false;
    }

    outputFile << jsonData.dump(4); // Запись с отступами
    outputFile.close();
    std::wcout << L"Измененный JSON записан в файл output.json." << std::endl;
    return true;
}



TJson readConf(std::string fileName)
{
    // if (fileName == "") { fileName = confFileName; }

    std::ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << fileName << std::endl;
        throw 1;
        return NULL;
    }

    // Читаем содержимое файла в строку
    // std::string jsonString((std::istreambuf_iterator<char>(inputFile)),
    //                         std::istreambuf_iterator<char>());
    std::stringstream ss;
    std::string line;
    while (std::getline(inputFile, line))
    {
        ss << line << std::endl;
    }
    std::string jsonString = ss.str();

    // Закрываем файл
    inputFile.close();

    // std::ifstream i("input.json");
    // auto j = nlohmann::ordered_json::parse(i);
    // std::cout << j.dump(2) << std::endl;

    // Парсим строку в объект JSON
    TJson jsonData;
    try
    {
        jsonData = TJson::parse(jsonString, nullptr, true, true);
    }
    catch (const TJson::parse_error &e)
    {
        std::wcerr << L"Ошибка парсинга JSON: " << e.what() << std::endl;
        throw 2;
        return NULL;
    }
    return jsonData;
}




// bool is_empty_str(const std::wstring* str) {
//     if (str == nullptr) {
//         return true;
//     } else if (str->empty()) {
//         return true;
//     } else {
//         return false;
//     }
// }


// bool is_empty_str(const char* str) {
//     if (str == nullptr) {
//         return true;
//     } else if (strlen(str) == 0) {
//         return true;
//     } else {
//         return false;
//     }
// }



/// @brief Форматирование строки
std::wstring format_str(std::wstring str, int width, wchar_t filler, int align)
{

    int widthReal = termColors.clearStr(str).size();
    // int w_fakt = utf8_length(wstringToString(str));
    if (widthReal >= width)
    {
        return str;
    }
    else
    {
        int w_add = width - widthReal;
        int part = (int)std::round((float)w_add/2.0f);
        std::wstringstream wss;
        switch (align)
        {
        case FORMAT_STR_CENTR:
            wss << std::setfill(filler) << std::setw(w_add - part) << "" << str << std::setw(part) << "" << std::setfill((wchar_t)' ');
            break;
        case FORMAT_STR_RIGHT:
            wss << std::setfill(filler) << std::setw(w_add) << filler << std::setfill((wchar_t)' ') << str;
            break;
        case FORMAT_STR_LEFT:
        default:
            wss << str << std::setfill(filler) << std::setw(w_add) << filler << std::setfill((wchar_t)' ');
            break;
        }
        return wss.str();
    }
}



std::wstring stringToWString(const std::string& str) {
    // Создаем конвертер для преобразования из UTF-8 в wide string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}



std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}



void replaceSubstring(std::wstring& str, const std::wstring& toReplace, const std::wstring& replacement)
{
    size_t pos = 0;
    // Ищем подстроку в строке
    while ((pos = str.find(toReplace, pos)) != std::wstring::npos) {
        // Заменяем подстроку
        str.replace(pos, toReplace.length(), replacement);
        // Переходим к следующей позиции
        pos += replacement.length();
    }
}



TVectorWString splitWStr(const std::wstring& str)
{
    TVectorWString words;
    std::wstringstream wss(str);
    std::wstring word;

    while (wss >> word) {
        words.push_back(word);
    }

    return words;
}



char** convertWStringVectorToCharArray(const TVectorWString& wstrVec) {
    // Создаем массив указателей на char
    char** args = new char*[wstrVec.size() + 1]; // +1 для завершающего nullptr

    // Создаем конвертер для преобразования wstring в string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    for (size_t i = 0; i < wstrVec.size(); ++i) {
        std::string str = converter.to_bytes(wstrVec[i]); // Преобразуем wstring в string
        args[i] = strdup(str.c_str()); // Копируем строку в char*
    }

    args[wstrVec.size()] = nullptr; // Завершаем массив nullptr

    return args;
}



void freeCharArray(char** args) {
    for (size_t i = 0; args[i] != nullptr; ++i)
    {
        free(args[i]);
    }
    delete[] args;
}



void clearScreen() {

    // ----
    // initscr();  // Инициализация ncurses
    // clear();    // Очистка экрана
    // refresh();  // Обновление экрана
    // endwin();   // Завершение работы с ncurses
    // ----
    // system("clear");
    // ----

    std::wcout << L"\033[2J\033[1;1H";

    // ----
}



void execSysCommand(std::wstring cmd)
{
    TVectorWString wstrArgs = splitWStr(cmd);
    char** charArgs = convertWStringVectorToCharArray(TVectorWString(wstrArgs.begin() + 0, wstrArgs.end()));
    // std::wcout << "cmd[" << wstringToString(wstrArgs[0]).c_str() << "]" << std::endl;
    // for (size_t i = 0; charArgs[i] != nullptr; ++i) { std::wcout << "p[" << charArgs[i] << "]" << std::endl; }
    // execv(wstringToString(wstrArgs[0]).c_str(), charArgs);
    execv(charArgs[0], charArgs);
    // Если exec вернется, значит произошла ошибка
    freeCharArray(charArgs);
    std::wcerr << L"При запуске [" << cmd << L"] что-то пошло не так." << std::endl;
    throw 10;
}



std::wstring insertHighLightHotKey(const std::wstring str)
{
    std::wstring pattern = std::wstring(1, CHAR_HIGHLIGHT) + L"(.)";
    std::wstring replacement = COLOR_HIGHLIGHT + L"$1" + termColors.off;
    std::wregex  re(pattern);
    std::wstring result = std::regex_replace(str, re, replacement);
    return result;
}


bool hasHighLighted(const std::wstring& text)
{
    std::wregex regex(termColors.pattern);
    return std::regex_search(text, regex);
}



void paramParser(int argc, char **argv)
{
    // Обработка аргументов
    // Например, если ожидается определенный аргумент-переключатель:
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            std::wcout
                << getHelp()
                << L"" << std::endl;
            return;
        }
        else if (arg == "--locale" || arg == "-l") {
            std::wcout << L"Используемая локаль: " << stringToWString(LOCALE) << std::endl;
            return;
        }
        // Другие обработки аргументов
        else if (arg == "--version" || arg == "-v") {
            std::wcout << getVersion() << std::endl;
            return;
        }
        // Можно добавить обработку других ключей или параметров
        else {
            std::wcout << L"Неизвестный параметр: " << arg.c_str() << std::endl;
        }
    }
}



std::string getLocale() {
    std::vector<std::string> locales;
    const char* command = "locale -a | grep ru | grep utf8";

    // Открываем процесс для чтения вывода команды
    std::unique_ptr<FILE, std::function<int(FILE*)>> pipe(popen(command, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Не удалось открыть процесс");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        // Убираем символ новой строки
        buffer[strcspn(buffer, "\n")] = 0;
        locales.push_back(buffer);
    }

    // std::cout << "Список системных локалей ru и utf8:" << std::endl;
    // for (int index = 0; index < locales.size(); ++index) {
    //     std::string& locale = locales[index];
    //     std::wcout << stringToWString(locale) << std::endl;
    // }
    // std::cout << "-----------------------------------" << std::endl;

    return locales[0];
}



#include <termios.h>
#include <unistd.h>

wchar_t my_getch() {
    struct termios oldt, newt;
    wchar_t ch;
    tcgetattr(STDIN_FILENO, &oldt); // Получаем текущие настройки терминала
    newt = oldt; // Копируем настройки
    newt.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Применяем новые настройки
    ch = getchar(); // Считываем символ
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Восстанавливаем старые настройки
    return ch;
}



#endif
