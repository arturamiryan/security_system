cmake_minimum_required(VERSION 3.0.0)
project(security_system VERSION 0.9.9)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lmysqlclient -L/usr/include/mysql -lpthread")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0) 
link_directories( ${GTKMM_LIBRARY_DIRS} )
include_directories( ${GTKMM_INCLUDE_DIRS} )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(security_system src/security_system.cpp src/main_window.cpp src/camera_drawing_area.cpp src/sql_con.cpp src/text_area.cpp src/emp_window.cpp src/FaceDetector.cpp)

target_link_libraries( security_system PRIVATE ${OpenCV_LIBS} ${GTKMM_LIBRARIES} )
target_include_directories( security_system PRIVATE include)

# Introduce preprocessor variables to keep paths of asset files
set(FACE_DETECTION_CONFIGURATION "${PROJECT_SOURCE_DIR}/assets/deploy.prototxt")
set(FACE_DETECTION_WEIGHTS "${PROJECT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")
target_compile_definitions(security_system PRIVATE FACE_DETECTION_CONFIGURATION="${FACE_DETECTION_CONFIGURATION}")
target_compile_definitions(security_system PRIVATE FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")
