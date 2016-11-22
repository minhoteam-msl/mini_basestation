#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T11:53:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demonstracao_basestation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ball.cpp \
    robo.cpp \
    desenharcampo.cpp \
    customscene.cpp \
    multicast.cpp \
    thpool.c \
    velocityvector.cpp

HEADERS  += mainwindow.h \
    ball.h \
    robo.h \
    desenharcampo.h \
    customscene.h \
    multicast.h \
    thpool.h \
    velocityvector.h

FORMS    += mainwindow.ui

INCLUDEPATH += /home/code/catkin_ws/src/minho_team_ros/coms_node/include
INCLUDEPATH += /home/code/catkin_ws/devel/include
INCLUDEPATH += /opt/ros/kinetic/include

RESOURCES += \
    rs.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lroscpp_serialization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lroscpp_serialization
else:unix: LIBS += -L$$PWD/./ -lroscpp_serialization
