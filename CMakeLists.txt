cmake_minimum_required(VERSION 3.5)

project(sm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sm
    src/consts.cpp
    src/consts.hpp
    src/functions.cpp
    src/functions.hpp
    src/main.cpp
    src/menu_elements.cpp
    src/menu_elements.hpp
)

# Команда выполняется после сборки цели sm
add_custom_command(
    TARGET sm
    POST_BUILD
    # Копирование файла.
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:sm>  # Полный путь к скомпилированному файлу (exe, dll, so и т.д.)
            "$ENV{HOME}/bin"     # Путь куда копировать (можно использовать абсолютный или относительный путь)

    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/output/

    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/conf/sm.conf.json ${CMAKE_SOURCE_DIR}/output/

    COMMENT "Копирование sm в директорию $ENV{HOME}/bin \n" +
            "Копирование sm в директорию ${CMAKE_SOURCE_DIR}/output/ \n" +
            "Копирование ${CMAKE_SOURCE_DIR}/conf/sm.conf.json в директорию ${CMAKE_SOURCE_DIR}/output/ \n"

)

include(GNUInstallDirs)
install(TARGETS sm
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
