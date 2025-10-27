cmake_minimum_required(VERSION 3.16)
project(QtYoloApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network) 
find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(TESSERACT REQUIRED tesseract)
find_library(LEPTONICA_LIBRARY NAMES lept leptonica REQUIRED)
find_path(LEPTONICA_INCLUDE_DIR NAMES leptonica/allheaders.h REQUIRED)

# Добавьте поиск библиотеки nlohmann/json
find_package(nlohmann_json REQUIRED)

qt_add_executable(QtYoloApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    yolo_processor.cpp
    yolo_processor.h
    settings_dialog.cpp 
    settings_dialog.h
    web_search_worker.cpp
    web_search_worker.h
    ocr_processor.cpp
    ocr_processor.h
)

target_link_libraries(QtYoloApp
    PRIVATE
        Qt6::Widgets
        Qt6::Network
        opencv_core
        opencv_dnn
        opencv_imgproc
        opencv_videoio
        opencv_imgcodecs
        ${CURL_LIBRARIES}
        ${TESSERACT_LIBRARIES}
        ${LEPTONICA_LIBRARY}
        nlohmann_json::nlohmann_json  # Добавьте эту строку
)

target_include_directories(QtYoloApp PRIVATE 
    ${OpenCV_INCLUDE_DIRS} 
    ${TESSERACT_INCLUDE_DIRS} 
    ${LEPTONICA_INCLUDE_DIR}
)