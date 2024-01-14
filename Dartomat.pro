QT       += core \
            multimedia \
            svgwidgets \
            widgets \
            texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 \ resources_big

RC_ICONS = img/dartomat.ico

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
    src/game_data_handler.cpp \
    src/cricket_class.cpp \
    src/cricket_input.cpp \
    src/cricket_mainwindow.cpp \
    src/dartboard_base.cpp \
    src/dartboard_cricket.cpp \
    src/dartboard_segment.cpp \
    src/dartboard_view.cpp \
    src/dartboard_x01.cpp \
    src/dartomat_mainwindow.cpp \
    src/dialog_name_input.cpp \
    src/groupbox_cricket.cpp \
    src/groupbox_x01.cpp \
    src/main.cpp \
    src/player_active_button.cpp \
    src/playerclass.cpp \
    src/spinbox.cpp \
    src/stats_window.cpp \
    src/x01_mainwindow.cpp \
    src/add_players_dialog.cpp

HEADERS += \
    inc/game_data_handler.h \
    inc/checkout_lists.h \
    inc/cricket_class.h \
    inc/cricket_input.h \
    inc/cricket_mainwindow.h \
    inc/dartboard_base.h \
    inc/dartboard_cricket.h \
    inc/dartboard_segment.h \
    inc/dartboard_view.h \
    inc/dartboard_x01.h \
    inc/dartomat_mainwindow.h \
    inc/dialog_name_input.h \
    inc/groupbox_cricket.h \
    inc/groupbox_x01.h \
    inc/player_active_button.h \
    inc/playerclass.h \
    inc/settings.h \
    inc/spinbox.h \
    inc/stats_window.h \
    inc/x01_mainwindow.h \
    inc/add_players_dialog.h

INCLUDEPATH = inc/

FORMS += \
    ui/add_players_dialog.ui \
    ui/cricket_input.ui \
    ui/cricket_mainwindow.ui \
    ui/dartomat_mainwindow.ui \
    ui/dialog_name_input.ui \
    ui/groupbox_cricket.ui \
    ui/groupbox_x01.ui \
    ui/stats_window.ui \
    ui/x01_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
