cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(barcode_reader)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(OpenCV REQUIRED)

include_directories(
    src/include
    ${OpenCV_INCLUDE_DIRS}
)

link_directories(
    ${OpenCV_LIBRARY_DIRS}
)

add_compile_options(-std=c++17 -O3 -g0)

add_executable (scan_test
    test/scan_test.cpp
    src/scan.cpp
    src/scan_utils.cpp
    src/image_proc.cpp
    src/config.cpp
)
    
target_link_libraries (scan_test
    -ltbb
    ${OpenCV_LIBS}
)

add_executable (scan_multiple
    test/scan_multiple.cpp
    src/scan.cpp
    src/scan_utils.cpp
    src/image_proc.cpp
    src/config.cpp
)
    
target_link_libraries (scan_multiple
    -ltbb
    ${OpenCV_LIBS}
)
    
add_executable (real_time
    test/real_time.cpp
    src/scan.cpp
    src/scan_utils.cpp
    src/image_proc.cpp
    src/config.cpp
)

target_link_libraries (real_time
    -ltbb
    ${OpenCV_LIBS}
)