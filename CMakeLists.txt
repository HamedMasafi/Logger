project(Logger)

set(LIB_SOURCES
  src/logconsole.cpp
  src/logconsole.h
  src/logger.cpp
  src/logger.h
  src/logmodel.cpp
  src/logmodel.h
  src/objectprofiller.cpp
  src/objectprofiller.h
  src/showlogdialog.cpp
  src/showlogdialog.h
  src/showlogdialog.ui
)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED
    COMPONENTS
    Widgets
    Core
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS
    Widgets
    Core
)

add_library(Logger_Static STATIC ${LIB_SOURCES})
add_library(Logger SHARED ${LIB_SOURCES})

target_link_libraries(Logger_Static PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_link_libraries(Logger PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
