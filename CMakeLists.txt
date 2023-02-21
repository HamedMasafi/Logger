project(Logger)

set(LIB_SOURCES
  src/logconsole.cpp
  include/Logger/logconsole.h
  src/logger.cpp
  include/Logger/logger.h
  src/logsmodel.cpp
  include/Logger/logsmodel.h
  include/Logger/manager.h
  src/manager.cpp
  include/Logger/logsproxymodel.h
  src/logsproxymodel.cpp
  src/objectprofiller.cpp
  include/Logger/objectprofiller.h
  include/Logger/log.h
  src/log.cpp
)

set(LIB_SOURCES_WIDGETS
    src/showlogdialog.cpp
    src/logdetails.cpp
    src/logsview.cpp
    include/Logger/showlogdialog.h
    include/Logger/showlogdialog.ui
    include/Logger/logdetails.h
    include/Logger/logdetails.ui
    include/Logger/logsview.h
)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 OPTIONAL_COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

add_library(Logger_Static STATIC ${LIB_SOURCES})
add_library(Logger SHARED ${LIB_SOURCES})

#Static library
target_link_libraries(Logger_Static PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(Logger_Static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(Qt${QT_VERSION_MAJOR}Widgets_FOUND)
    target_sources(
        Logger_Static PRIVATE
        ${LIB_SOURCES_WIDGETS}
    )
    target_link_libraries(Logger_Static PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
    )
endif()

#Shared library

target_link_libraries(Logger PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(Qt${QT_VERSION_MAJOR}Widgets_FOUND)
    target_sources(
        Logger PRIVATE
        ${LIB_SOURCES_WIDGETS}
    )
    target_link_libraries(Logger PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
    )
endif()
