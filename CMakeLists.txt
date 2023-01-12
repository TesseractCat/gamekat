cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(GameKeeb CXX C)

pico_sdk_init()

# Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
else()
    message("Git not found or not a git repo")
endif()

include_directories(include pico-joybus-comms/include)

add_executable(GameKeeb
    src/main.cpp
    pico-joybus-comms/src/joybus.cpp
)

pico_generate_pio_header(GameKeeb ${CMAKE_CURRENT_LIST_DIR}/pico-joybus-comms/pio/joybus.pio)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/joybus.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/joybus.pio
        COMMAND Pioasm ${CMAKE_CURRENT_LIST_DIR}/joybus.pio ${CMAKE_CURRENT_LIST_DIR}/generated/joybus.pio.h
        )


target_link_libraries(GameKeeb pico_stdlib pico_platform hardware_pio pico_bootrom pico_multicore hardware_resets hardware_irq tinyusb_host tinyusb_board hardware_flash)

pico_add_extra_outputs(GameKeeb)
