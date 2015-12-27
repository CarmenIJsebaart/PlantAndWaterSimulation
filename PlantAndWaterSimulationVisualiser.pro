QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Werror

SOURCES += \
    qtmain.cpp \
    qtwidget.cpp \
    simulation.cpp \
    print.cpp \
    grid.cpp

FORMS += \
    qtwidget.ui

HEADERS += \
    qtwidget.h \
    simulation.h \
    print.h \
    grid.h

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG
}

