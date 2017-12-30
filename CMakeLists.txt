cmake_minimum_required(VERSION 3.8)
project(b_suitors)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES "src/*.h" "src/*.cpp" "src/*.hpp")
add_executable(adorate ${SOURCE_FILES} blimits/blimit.cpp)
add_executable(example ${SOURCE_FILES} blimits/blimit.cpp)
add_executable(junix_dblp ${SOURCE_FILES} blimits/blimit-dblp.cpp)
add_executable(junix_facebook_combined ${SOURCE_FILES} blimits/blimit-facebook_combined.cpp)
add_executable(junix_paper ${SOURCE_FILES} blimits/blimit-paper.cpp)
add_executable(junix_youtube ${SOURCE_FILES} blimits/blimit-youtube.cpp)
add_executable(junix_road_pa ${SOURCE_FILES} blimits/blimit-road_pa.cpp)
add_executable(kacper ${SOURCE_FILES} blimits/blimit-kacper.cpp)