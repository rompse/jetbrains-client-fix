cmake_minimum_required(VERSION 3.21)
project(jetbrains_client_fix)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        MinHook
        GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
        GIT_TAG master
)
FetchContent_MakeAvailable(MinHook)

add_library(jetbrains_client_fix SHARED main.cpp)
target_include_directories(jetbrains_client_fix PRIVATE minhook)
target_link_libraries(jetbrains_client_fix PRIVATE minhook)
