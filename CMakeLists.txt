cmake_minimum_required(VERSION 3.15)
project(ForkerEngine)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Macro
#######
macro(makeLink src dest target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}
        DEPENDS ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

# Find the required packages
############################
find_package(GLM REQUIRED)
message(STATUS "Found GLM in: ${GLM_INCLUDE_DIRS}")

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in: ${GLFW3_INCLUDE_DIRS}")

find_package(ASSIMP REQUIRED)
message(STATUS "Found Assimp in: ${ASSIMP_INCLUDE_DIRS}")


# Static Libraries
############################
# Creates relevant static libraries (PUBLIC - no need to include the headers in other targets)
add_library(stb_image STATIC src/vendor/stb_image/stb_image.cpp)
add_library(glad STATIC src/vendor/glad/glad.c)
#add_library(filesystem STATIC src/vendor/filesystem/filesystem.cpp)
add_library(imgui STATIC src/vendor/imgui/imgui.cpp
    src/vendor/imgui/imgui_demo.cpp src/vendor/imgui/imgui_draw.cpp
    src/vendor/imgui/imgui_impl_glfw.cpp src/vendor/imgui/imgui_impl_opengl3.cpp
    src/vendor/imgui/imgui_tables.cpp src/vendor/imgui/imgui_widgets.cpp)

target_include_directories(stb_image
    PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(glad
    PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(filesystem
#    PRIVATE ${CMAKE_SOURCE_DIR}/include/learnopengl ${CMAKE_BINARY_DIR}/configuration)
target_include_directories(imgui
    PRIVATE ${CMAKE_SOURCE_DIR}/include/imgui ${CMAKE_SOURCE_DIR}/include)

# Set include directories
############################
set(INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include)


# Set libraries
############################
set(LIBS
    ${LIBS}
    ${GLFW3_LIBRARY}
    ${ASSIMP_LIBRARY}
    stb_image # target name
    glad
    #    filesystem
    imgui)

if (APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(APPLE_LIBS
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKit_LIBRARY}
        ${CoreVideo_LIBRARY})
    set(LIBS ${LIBS} ${APPLE_LIBS})
    # https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt
endif ()


# Targets
############################
add_executable(
    ForkerEngine

    src/program.cpp

    # engine
    src/engine/forker_engine.cpp

    src/engine/resource/resource_manager.cpp
    src/engine/resource/shader.cpp
    src/engine/resource/texture.cpp

    src/engine/gui/gui.cpp

    src/engine/camera.cpp
)
target_include_directories(ForkerEngine PRIVATE ${INCLUDE_DIRS})
target_link_libraries(ForkerEngine PRIVATE ${LIBS})


# Other targets
#foreach (CHAPTER ${CHAPTERS})
#    foreach (DEMO ${${CHAPTER}})
#        file(
#            GLOB SOURCE
#            src/${CHAPTER}/${DEMO}/*.h
#            src/${CHAPTER}/${DEMO}/*.cpp
#            src/${CHAPTER}/${DEMO}/*.vs
#            src/${CHAPTER}/${DEMO}/*.fs
#            src/${CHAPTER}/${DEMO}/*.gs
#            src/${CHAPTER}/${DEMO}/*.vert
#            src/${CHAPTER}/${DEMO}/*.frag
#            src/${CHAPTER}/${DEMO}/*.geom
#        )
#        set(TARGET_NAME ${CHAPTER}__${DEMO})
#        add_executable(${TARGET_NAME} ${SOURCE})
#        # header
#        target_include_directories(${TARGET_NAME}
#            PRIVATE ${INCLUDE_DIRS})
#        # link
#        target_link_libraries(${TARGET_NAME}
#            PRIVATE ${LIBS})
#
#        # copy shader files to build directory
#        file(
#            GLOB SHADERS
#            src/${CHAPTER}/${DEMO}/*.vs
#            src/${CHAPTER}/${DEMO}/*.fs
#            src/${CHAPTER}/${DEMO}/*.gs
#            src/${CHAPTER}/${DEMO}/*.vert
#            src/${CHAPTER}/${DEMO}/*.frag
#            src/${CHAPTER}/${DEMO}/*.geom
#        )
#        foreach (SHADER ${SHADERS})
#            if (APPLE)
#                # create symbolic link for those files
#                get_filename_component(SHADER_NAME ${SHADER} NAME)
#                makeLink(${SHADER}
#                    ${CMAKE_SOURCE_DIR}/cmake-build-debug/${SHADER_NAME}
#                    ${TARGET_NAME})
#            endif ()
#        endforeach ()
#
#        # copy breakout shaders
#        file(
#            GLOB BREAKOUT_SHADERS
#            src/${CHAPTER}/${DEMO}/shaders/*
#        )
#        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-debug/shaders)
#        foreach (SHADER ${BREAKOUT_SHADERS})
#            if (APPLE)
#                # create symbolic link for those files
#                get_filename_component(SHADER_NAME ${SHADER} NAME)
#                makeLink(${SHADER}
#                    ${CMAKE_SOURCE_DIR}/cmake-build-debug/shaders/${SHADER_NAME}
#                    ${TARGET_NAME})
#            endif ()
#        endforeach ()
#
#        # copy IrrKlang plugin files
#        file(
#            GLOB PLUGIN_FILES
#            dlls/*
#        )
#        foreach (PLUGIN_PATH ${PLUGIN_FILES})
#            if (APPLE)
#                # create symbolic link for those files
#                get_filename_component(PLUGIN_FILE_NAME ${PLUGIN_PATH} NAME)
#                makeLink(${PLUGIN_PATH}
#                    ${CMAKE_SOURCE_DIR}/cmake-build-debug/${PLUGIN_FILE_NAME}
#                    ${TARGET_NAME})
#            endif ()
#        endforeach ()
#
#    endforeach ()
#endforeach ()
