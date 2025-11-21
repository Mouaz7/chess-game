cmake_minimum_required(VERSION 3.16)
project(Schack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Hitta SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Lägg till källfiler
add_executable(Schack
    main.cpp
)

# Länka SFML bibliotek
target_link_libraries(Schack PRIVATE sfml-graphics sfml-window sfml-system)

# För Windows, kopiera SFML DLL:er
if(WIN32)
    add_custom_command(TARGET Schack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:Schack>
    )
endif()
