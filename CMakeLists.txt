cmake_minimum_required(VERSION 3.25)

project(sterm
    VERSION 0.1.0
    DESCRIPTION "Simple Terminal - A coroutine-based terminal application"
    LANGUAGES CXX
)

# C++23 standard required for Boost.Cobalt
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Find required packages
find_package(Boost REQUIRED COMPONENTS cobalt)
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(STERM_SOURCES
    src/main.cpp
    src/terminal.cpp
    src/command_handler.cpp
    src/config.cpp
    src/output_manager.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${STERM_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Boost::cobalt
        Threads::Threads
)

# Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Tests (optional)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# =============================================================================
# CPack configuration for package generation
# =============================================================================

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_DESCRIPTION "A coroutine-based terminal application written in C++23 using Boost.Cobalt. Features asynchronous command execution, thread-safe output handling, and command history support.")
set(CPACK_PACKAGE_VENDOR "sterm")
set(CPACK_PACKAGE_CONTACT "maintainer@example.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/YOUR_USERNAME/sterm")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# DEB package specific settings
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <maintainer@example.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-all-dev (>= 1.84)")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

# Package output directory
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/packages")

# Include CPack module
include(CPack)
