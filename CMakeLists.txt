cmake_minimum_required(VERSION 3.11)

project(stepik
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if ( UNIX )
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb -O0 -fsanitize=address -DEBUG")
    #set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb -O0 -DEBUG")
endif()

add_compile_options(-Wall -Wextra -fdiagnostics-color)

include_directories(include/ src/)

link_libraries(asan)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

list(REMOVE_ITEM SRC_FILES "${SRC_DIR}/constexpr.cpp")

foreach(SRC_FILE ${SRC_FILES})
    get_filename_component(NAME ${SRC_FILE} NAME_WE)

    add_executable(${NAME} ${SRC_FILE})
endforeach()

set(PROPERTIES_TARGETS 
    "to_pair"
    "perfect_forwarding"
    "associative_containers"
    "iterators"
    "algorithms"
    "async"
)

foreach(TGT ${PROPERTIES_TARGETS})
    #message(${TGT})
    set_target_properties(${TGT} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )
endforeach()

add_executable(constexpr_test tests/constexpr_test.cpp)
target_link_libraries(constexpr_test gtest gmock pthread)
