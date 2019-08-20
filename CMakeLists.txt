cmake_minimum_required(VERSION 3.1)

set(project_name hls-example) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/Utils.cpp
        src/Utils.hpp
        src/controller/MediaController.cpp
        src/controller/MediaController.hpp
        src/hls/Playlist.cpp
        src/hls/Playlist.hpp
)

target_include_directories(${project_name}-lib
        PUBLIC src
)

## link libs

find_package(oatpp 0.19.8 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
)

add_definitions(
        -DEXAMPLE_MEDIA_FOLDER="${CMAKE_CURRENT_LIST_DIR}/video"
)

## add executables

add_executable(${project_name}-exe
        src/App.cpp
)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
)
target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

enable_testing()
add_test(project-tests ${project_name}-test)
