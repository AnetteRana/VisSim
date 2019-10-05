QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programmering

SOURCES += main.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    axis.cpp \
    graph2d.cpp \
    tetrahedron.cpp \
    star.cpp \
    vertex.cpp \
    vector3d.cpp \
    matrix4x4.cpp \
    vector4d.cpp \
    vector2d.cpp \
    VisualObject.cpp \
    plane.cpp \
    graph3d.cpp \
    trianglesurface.cpp \
    interactiveobject.cpp \
    cube.cpp \
    camera.cpp \
    octahedronball.cpp \
    collision.cpp \
    collisionboxaabb.cpp \
    collisionsphere.cpp \
    recursivetriangles.cpp \
    beziercurve.cpp \
    stb_image.cpp \ 
    objmesh.cpp

HEADERS += \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    axis.h \
    graph2d.h \
    tetrahedron.h \
    star.h \
    vertex.h \
    vector3d.h \
    matrix4x4.h \
    vector4d.h \
    vector2d.h \
    VisualObject.h \
    plane.h \
    graph3d.h \
    trianglesurface.h \
    interactiveobject.h \
    cube.h \
    camera.h \
    octahedronball.h \
    collision.h \
    collisionboxaabb.h \
    collisionsphere.h \
    recursivetriangles.h \
    beziercurve.h \
    rmath.h \
    stb_image.h \ 
    objmesh.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert \
    texturefragment.frag \
    texturevertex.vert \
    phong.frag \
    phong.vert \
    phong.frag \
    phong.vert \
    height.vert \
    height.frag
