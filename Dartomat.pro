QT       += core gui \
            multimedia \
            svg \
            charts \
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \ resources_big

RC_ICONS = dartomat.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cricketclass.cpp \
    cricketinput.cpp \
    dartboardsegment.cpp \
    dialognameinput.cpp \
    gamewindow.cpp \
    groupbox_cricket.cpp \
    groupbox_player.cpp \
    main.cpp \
    dartomatmain.cpp \
    myqspinbox.cpp \
    playeractivebutton.cpp \
    playerclass.cpp \
    scoreinput.cpp \
    statswindow.cpp \
    submitbutton.cpp \
    undobutton.cpp

HEADERS += \
    cricketclass.h \
    cricketinput.h \
    dartboardsegment.h \
    dartomatmain.h \
    dialognameinput.h \
    gamewindow.h \
    groupbox_cricket.h \
    groupbox_player.h \
    myqspinbox.h \
    playeractivebutton.h \
    playerclass.h \
    scoreinput.h \
    statswindow.h \
    submitbutton.h \
    undobutton.h

FORMS += \
    dartomatmain.ui \
    dialognameinput.ui \
    gamewindow.ui \
    groupbox_cricket.ui \
    groupbox_player.ui \
    scoreinput.ui \
    statswindow.ui \
    cricketinput.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
