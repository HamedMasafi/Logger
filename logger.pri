INCLUDEPATH += $$PWD/src

DEFINES += KAJ_LOGGER_LIB
HEADERS += \
    $$PWD/src/objectprofiller.h \
    $$PWD/src/logger.h \
    $$PWD/src/logmodel.h \
    $$PWD/src/showlogdialog.h \
    $$PWD/src/logconsole.h

SOURCES += \
    $$PWD/src/objectprofiller.cpp \
    $$PWD/src/logger.cpp \
    $$PWD/src/logmodel.cpp \
    $$PWD/src/showlogdialog.cpp \
    $$PWD/src/logconsole.cpp


contains(QT, widgets) {
    FORMS += $$PWD/src/showlogdialog.ui
}
