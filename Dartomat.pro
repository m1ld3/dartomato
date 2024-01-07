QT       += core \
            multimedia \
            svgwidgets \
            widgets \
            charts

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
    cdartboard_base.cpp \
    chart.cpp \
    chartview.cpp \
    cricketclass.cpp \
    cricketinput.cpp \
    cricketmainwindow.cpp \
    dartboard.cpp \
    dartboard_cricket.cpp \
    dartboardsegment.cpp \
    dartboardview.cpp \
    dialognameinput.cpp \
    groupbox_cricket.cpp \
    groupbox_player.cpp \
    main.cpp \
    dartomatmain.cpp \
    myqspinbox.cpp \
    playeractivebutton.cpp \
    playerclass.cpp \
    statswindow.cpp \
    undobutton.cpp \
    xo1mainwindow.cpp

HEADERS += \
    cdartboard_base.h \
    chart.h \
    chartview.h \
    checkoutlists.h \
    cricketclass.h \
    cricketinput.h \
    cricketmainwindow.h \
    dartboard.h \
    dartboard_cricket.h \
    dartboardsegment.h \
    dartboardview.h \
    dartomatmain.h \
    dialognameinput.h \
    groupbox_cricket.h \
    groupbox_player.h \
    myqspinbox.h \
    playeractivebutton.h \
    playerclass.h \
    settings.h \
    statswindow.h \
    undobutton.h \
    xo1mainwindow.h

FORMS += \
    dartomatmain.ui \
    dialognameinput.ui \
    groupbox_cricket.ui \
    groupbox_player.ui \
    statswindow.ui \
    cricketinput.ui \
    cricketmainwindow.ui \
    xo1mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
