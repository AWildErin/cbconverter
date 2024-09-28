cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(cbconverter)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cbreader")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/argparse")


set(${PROJECT_NAME}_SRC
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_include_directories(
    ${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    CBReader
    argparse::argparse
)
