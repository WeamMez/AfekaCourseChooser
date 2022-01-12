QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    course.cpp \
    courseinstance.cpp \
    courseinstancetimeslot.cpp \
    main.cpp \
    mainwindow.cpp \
    newcoursedialog.cpp \
    newinstancedialog.cpp \
    schedule.cpp \
    schedulefilehandler.cpp \
    schedulemanager.cpp

HEADERS += \
    course.h \
    courseinstance.h \
    courseinstancetimeslot.h \
    mainwindow.h \
    newcoursedialog.h \
    newinstancedialog.h \
    schedule.h \
    schedulefilehandler.h \
    schedulemanager.h

FORMS += \
    mainwindow.ui \
    newcoursedialog.ui \
    newinstancedialog.ui

TRANSLATIONS += \
    CourseChooser_he_IL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
