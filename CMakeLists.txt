cmake_minimum_required(VERSION 3.16)
project(game VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

set (CMAKE_CXX_STANDARD 20)
include(CMakePrintHelpers)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_subdirectory("third_party/raylib")
add_subdirectory("third_party/ldtk")
add_subdirectory("third_party/imgui")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib imgui LDtkLoader::LDtkLoader)

# copies the assets folder relative to the CMakeLists.txt file to the folder relative to the "out/build/GCC..." path
file(COPY "assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")

target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
#target_compile_definitions(${PROJECT_NAME} PUBLIC RESOURCES_PATH="./assets/") # Uncomment this and comment the one above when shipping !!!