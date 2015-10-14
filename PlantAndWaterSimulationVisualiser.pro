QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++ -Werror

SOURCES += \
    qtmain.cpp \
    qtwidget.cpp \
    simulation.cpp \
    grid.cpp

FORMS += \
    qtwidget.ui

HEADERS += \
    qtwidget.h \
    simulation.h \
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


