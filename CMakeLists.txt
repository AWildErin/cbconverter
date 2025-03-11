cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(cbconverter)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cbreader")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/argparse")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog")

set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF) # @todo not sure if this is a good approach

set(ASSIMP_BUILD_FBX_EXPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_EXPORTER OFF) # @todo Would like this in the future, but I get compile errors
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp")


set(${PROJECT_NAME}_SRC
	${CMAKE_CURRENT_LIST_DIR}/include/cbconverter/exporter.h
	${CMAKE_CURRENT_LIST_DIR}/src/exporter.cpp

	${CMAKE_CURRENT_LIST_DIR}/include/cbconverter/export_rmesh.h
	${CMAKE_CURRENT_LIST_DIR}/src/export_rmesh.cpp

	${CMAKE_CURRENT_LIST_DIR}/include/cbconverter/utils.h
	${CMAKE_CURRENT_LIST_DIR}/src/utils.cpp

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
    assimp
	spdlog
)
