cmake_minimum_required(VERSION 2.8)

project(projet_pc_si4)

add_definitions("-g -ansi -Wall -Wextra -Wold-style-cast -std=c++1z")

add_executable(main
	Cell.cpp
	Map.cpp
	Person.cpp
	main.cpp
)

target_link_libraries(main
	pthread)

install(TARGETS
        main
        RUNTIME DESTINATION .)
