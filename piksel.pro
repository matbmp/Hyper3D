QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game/game.cpp \
    Game/gameobject.cpp \
    Graphics/Color.cpp \
    Graphics/interpolation.cpp \
    Graphics/lighting.cpp \
    Graphics/lightsource.cpp \
    Graphics/mesh.cpp \
    Math.cpp \
    Math/Matrix3.cpp \
    Math/Matrix4.cpp \
    Math/Point2.cpp \
    Math/Vector2.cpp \
    Math/Vector3.cpp \
    Math/Vector4.cpp \
    Math/quaternion.cpp \
    main.cpp \
    mywindow.cpp

HEADERS += \
    Game/Genaration.h \
    Game/HMath.h \
    Game/InputManager.h \
    Game/game.h \
    Game/gameobject.h \
    Graphics.h \
    Graphics/Color.h \
    Graphics/Sampler.h \
    Graphics/Shader.h \
    Graphics/cpugraphicsdevice.h \
    Graphics/interpolation.h \
    Graphics/lighting.h \
    Graphics/lightsource.h \
    Graphics/mesh.h \
    Graphics/rendertarget.h \
    Graphics/vertex.h \
    Math.h \
    Math/Matrix3.h \
    Math/Matrix4.h \
    Math/Point2.h \
    Math/Vector2.h \
    Math/Vector3.h \
    Math/Vector4.h \
    Math/quaternion.h \
    mywindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
