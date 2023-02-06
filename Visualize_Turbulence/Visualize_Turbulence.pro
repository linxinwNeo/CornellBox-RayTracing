QT       += core gui openglwidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    openglwindow.cpp

HEADERS += \
    AABB.h \
    BVH.h \
    Box.h \
    Camera.h \
    Hittable.h \
    Hittable_List.h \
    Material.h \
    Moving_Sphere.h \
    Perlin.h \
    Ray.h \
    Rtweekend_stb_image.h \
    Sphere.h \
    aarect.h \
    color.h \
    constant_medium.h \
    mainwindow.h \
    openglwindow.h \
    rtweekend.h \
    stb_image.h \
    texture.h \
    vec3.h

FORMS += \
    mainwindow.ui

mac: LIBS += -framework GLUT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    res.qrc
