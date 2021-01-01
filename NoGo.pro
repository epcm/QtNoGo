QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ai/aimcts.cpp \
    gamewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    referee.cpp \
    welcomewidget.cpp

HEADERS += \
    ai/aimcts.h \
    gamewidget.h \
    mainwindow.h \
    referee.h \
    utils.h \
    welcomewidget.h

FORMS += \
    gamewidget.ui \
    welcomewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

