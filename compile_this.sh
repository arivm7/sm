#!/bin/bash

##
## Компилирование файла без использования IDE среды
##

DIR_BUILD="build"

if [ -d "${DIR_BUILD}" ]; then
    printf "Папка [%s] есть.\n" "${DIR_BUILD}"
else
    printf "Создание папки [%s].\n" "${DIR_BUILD}"
    mkdir build
fi
cd "${DIR_BUILD}" || { echo "По какой-то причине не удалось прейти в папку [${DIR_BUILD}]"; exit 1; }

cmake ..
cmake --build . --target all --config Release

