# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.28.1)

# Define the project name
project(Spliced)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

# Define the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")

# Add the executable
add_executable(Spliced ${SOURCES})

# Download all Git submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule u`date --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# Check that all the submodules downloaded
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The GLFW submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glad/cmake/CMakeLists.txt")
    message(FATAL_ERROR "The Glad submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glm/glm/CMakeLists.txt")
    message(FATAL_ERROR "The GLM submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/stb/stb_image.h")
    message(FATAL_ERROR "The STB submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# Ensure pip + jinja2 is installed
find_package(Python3 COMPONENTS Interpreter REQUIRED)
execute_process(
    COMMAND ${Python3_EXECUTABLE} -m ensurepip
    RESULT_VARIABLE ENSUREPIP_RESULT
)
if(NOT ENSUREPIP_RESULT EQUAL "0")
    message(FATAL_ERROR "Failed to ensure pip is installed. Please check your Python installation.")
endif()
execute_process(
    COMMAND ${Python3_EXECUTABLE} -m pip install --user jinja2
    RESULT_VARIABLE JINJA2_INSTALL_RESULT
)
if(NOT JINJA2_INSTALL_RESULT EQUAL "0")
    message(FATAL_ERROR "Failed to install jinja2. Please ensure pip is installed and try again.")
endif()

# Include directories for header files
target_include_directories(Spliced PRIVATE ${CMAKE_SOURCE_DIR}/src
                                            ${CMAKE_SOURCE_DIR}/external/glm
                                            ${CMAKE_SOURCE_DIR}/external/stb
                                            ${CMAKE_SOURCE_DIR}/includes
                                            # Glad builds its include folder to the build directory
                                            ${CMAKE_BINARY_DIR}/gladsources/glad/include/glad)

# Process the cmake files in these subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad/cmake)

# Function defined in glad cmake file
glad_add_library(glad STATIC API gl:core=4.1)

# Link libraries
target_link_libraries(Spliced glfw glad ${CMAKE_DL_LIBS})