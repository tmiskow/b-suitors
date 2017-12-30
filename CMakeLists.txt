cmake_minimum_required(VERSION 3.8)
project(b_suitors)

set(CMAKE_CXX_STANDARD 17)

find_package ( Threads REQUIRED )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${CMAKE_THREAD_LIBS_INIT} )
    endif()
endmacro()

file(GLOB SOURCE_FILES "src/*.h" "src/*.cpp" "src/*.hpp")
add_executable(adorate ${SOURCE_FILES} blimits/blimit.cpp)
add_executable(example ${SOURCE_FILES} blimits/blimit.cpp)
add_executable(junix_dblp ${SOURCE_FILES} blimits/blimit-dblp.cpp)
add_executable(junix_facebook_combined ${SOURCE_FILES} blimits/blimit-facebook_combined.cpp)
add_executable(junix_paper ${SOURCE_FILES} blimits/blimit-paper.cpp)
add_executable(junix_youtube ${SOURCE_FILES} blimits/blimit-youtube.cpp)
add_executable(junix_road_pa ${SOURCE_FILES} blimits/blimit-road_pa.cpp)
add_executable(kacper ${SOURCE_FILES} blimits/blimit-kacper.cpp)
add_executable(klika ${SOURCE_FILES} blimits/blimit-klika.cpp)

install(TARGETS DESTINATION .)