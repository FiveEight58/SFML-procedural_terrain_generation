cmake_minimum_required(VERSION 3.25)
set(NAME PROC_GEN)
project(${NAME})


#Linux
if (CMAKE_HOST_SYSTEM MATCHES Linux)
 
    file(GLOB SOURCES "src/*.cpp" "lib/SFML-2.6.0/lib/*.a")

    include_directories("./lib/SFML-2.6.0/lib/include")
    message(STATUS "Target system is Linux")
    link_directories("./lib/SFML-2.6.0/lib")

    add_executable(${NAME} ${SOURCES})
 
    target_link_libraries(
        ${NAME}
        sfml-graphics
        sfml-window
        sfml-system
    )


endif()


#Windows
if (CMAKE_HOST_SYSTEM MATCHES Windows)
 
    file(GLOB SOURCES "src/*.cpp")
 
    include_directories("./lib/SFML-2.6.0/include")
    message(STATUS "Target system is Windows")
    link_directories("./lib/SFML-2.6.0/winlib")
 
    add_executable(${NAME} ${SOURCES})
 
    target_link_libraries(
        ${NAME}
        sfml-graphics
        sfml-window
        sfml-system
    )
 
endif()

#Installation (optional)
#install(TARGETS ${NAME})
