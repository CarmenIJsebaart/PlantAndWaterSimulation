TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    grid.cpp \
    print.cpp \
    simulation.cpp

HEADERS += \
  grid.h \
  print.h \
  simulation.h

QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror

#LIBS += -lopencv

QMAKE_CXXFLAGS += -std=c++11

include(deployment.pri)
qtcAddDeployment()
