cmake_minimum_required (VERSION 3.0)
project(drm-init-gamma)

include (GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

find_package(PkgConfig)
pkg_check_modules(DRM REQUIRED libdrm)

add_subdirectory(src)
