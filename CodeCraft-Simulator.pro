
HEADERS += mainwindow.h view.h \
    roadline.h \
    carRect.h \
    crossRect.h \
    codecraft.h
SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    roadline.cpp \
    carRect.cpp \
    crossRect.cpp \
    codecraft.cpp

QT += widgets
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/chip
INSTALLS += target

RESOURCES += \
    inputfile.qrc

DISTFILES +=
