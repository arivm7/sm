#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <cstdlib>
// #include <fstream>
// #include <iostream>
// #include <codecvt>
#include <nlohmann/json.hpp>
#include "consts.hpp"
using TJson = nlohmann::ordered_json;
using TVectorWString = std::vector<std::wstring>;


size_t utf8_length(const std::string& str);

bool fileExist(const std::string& filename);

bool fileRemove(const std::string& filename);

bool fileRename(const char* oldName, const char* newName);

/// @brief Запись JSON в файл
/// @param data 
/// @param fileName -- имя файла для 
/// @return
bool saveConf(TJson& jsonData, std::string fileName = CONF_FILE_NAME);

TJson readConf(std::string fileName = CONF_FILE_NAME);

template <typename T>
bool is_null(T* ptr) {
    return ptr == nullptr;
}

template <typename T>
bool is_null(T ptr) {
    return *ptr == nullptr;
}

/// @brief Форматирование строки
std::wstring format_str(std::wstring str, int width, wchar_t filler, int align);

std::wstring stringToWString(const std::string& str);

std::string wstringToString(const std::wstring& wstr);

void replaceSubstring(std::wstring& str, const std::wstring& toReplace, const std::wstring& replacement);

TVectorWString splitWStr(const std::wstring& str);

char** convertWStringVectorToCharArray(const TVectorWString& wstrVec);

void freeCharArray(char** args);

void clearScreen();

void execSysCommand(std::wstring cmd);

std::wstring insertHighLightHotKey(const std::wstring str);

bool hasHighLighted(const std::wstring& text);

void paramParser(int argc, char **argv);

std::string getLocale();

wchar_t my_getch();



#endif
