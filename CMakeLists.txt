cmake_minimum_required(VERSION 3.16)

project(SalesApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp

    models/user.h
    models/user.cpp

    controllers/authcontroller.h
    controllers/authcontroller.cpp
    controllers/productcontroller.h
    controllers/productcontroller.cpp

    views/loginwindow.h
    views/loginwindow.cpp

    views/mainwindow.h
    views/mainwindow.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SalesApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        models/product.h models/product.cpp
        repositories/productrepository.h repositories/productrepository.cpp
        views/productswindow.h views/productswindow.cpp
        views/productswindow.h
        models/customer.h models/customer.cpp
        repositories/customerrepository.h repositories/customerrepository.cpp
        controllers/customercontroller.h controllers/customercontroller.cpp
        views/customerswindow.h views/customerswindow.cpp
        models/deal.h models/deal.cpp
        repositories/dealrepository.h repositories/dealrepository.cpp
        controllers/dealcontroller.h controllers/dealcontroller.cpp
        views/dealswindow.h views/dealswindow.cpp
    )
else()
    add_executable(SalesApp
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(SalesApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(SalesApp PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SalesApp)
endif()
