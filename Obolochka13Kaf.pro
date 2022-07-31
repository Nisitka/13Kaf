QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    literature.cpp \
    main.cpp \
    matchthreadrezult.cpp \
    menu.cpp \
    setings.cpp \
    testing.cpp \
    rezult.cpp \
    matchrezultobject.cpp \
    setuserform.cpp \
    winerror.cpp

HEADERS += \
    app.h \
    literature.h \
    matchthreadrezult.h \
    menu.h \
    setings.h \
    sizeImage.h \
    testing.h \
    rezult.h \
    testitem.h \
    setcoloritem.h \
    matchrezultobject.h \
    theme.h \
    setuserform.h \
    winerror.h

FORMS += \
    literature.ui \
    menu.ui \
    setings.ui \
    testing.ui \
    rezult.ui \
    setuserform.ui \
    winerror.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += sql

RESOURCES +=

DISTFILES +=
