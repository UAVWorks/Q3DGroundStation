#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T12:46:01
#
#-------------------------------------------------

QT       += core gui svg serialport network webengine webenginewidgets webchannel websockets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q3DGroundStation
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32: DEFINES += WIN32 _WINDOWS _USE_MATH_DEFINES


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Connection/connection.cpp \
    3D/qt_osg_widget.cpp \
    3D/QtOSG/OSGWidget.cpp \
    3D/QtOSG/PickHandler.cpp \
    Meters/meters.cpp \
    Meters/QMeter/qmeter.cpp \
    Meters/QMeter/qmeterdrawfunctions.cpp \
    HUD/hud.cpp \
    Meters/QGauge/qgauge.cpp \
    Meters/QGauge/qgaugedrawfunctions.cpp \
    Curve/data_curve.cpp \
    Meters/QVUMeter/qvumeter.cpp \
    Progress/progress.cpp \
    Map/map.cpp \
    Warn/warn_item.cpp \
    Warn/warning.cpp \
    Compass/compass_widget.cpp \
    HUD/hud_view.cpp \
    FlightControl/flight_control.cpp \
    TachoMeter/tachometer.cpp \
    shared/websocketclientwrapper.cpp \
    shared/websockettransport.cpp \
    AttitudeCtl/attitude_control.cpp \
    Connection/mavlink_protocol.cpp \
    ValueControl/value_control.cpp \
    Uav/uav.cpp \
    cmd_param.cpp \
    FlightControl/track_planning.cpp \
    Connection/serial_read_thread.cpp \
    Connection/serial_write_thread.cpp \
    Meters/Vsi/vsi.cpp \
    Meters/Alt/alt.cpp

HEADERS += \
        mainwindow.h \
    Connection/connection.h \
    3D/qt_osg_widget.h \
    3D/QtOSG/OSGWidget.h \
    3D/QtOSG/PickHandler.h \
    Meters/meters.h \
    Meters/QMeter/qmeter.h \
    HUD/hud.h \
    Meters/QGauge/qgauge.h \
    Curve/data_curve.h \
    Meters/QVUMeter/qvumeter.h \
    Progress/progress.h \
    Map/map.h \
    Warn/warn_item.h \
    Warn/warning.h \
    Compass/compass_widget.h \
    HUD/hud_view.h \
    FlightControl/flight_control.h \
    TachoMeter/tachometer.h \
    shared/websocketclientwrapper.h \
    shared/websockettransport.h \
    Map/mapchannel.h \
    AttitudeCtl/attitude_control.h \
    Connection/mavlink_protocol.h \
    q3dgs.h \
    ValueControl/value_control.h \
    Uav/uav.h \
    cmd_param.h \
    FlightControl/track_planning.h \
    Connection/serial_read_thread.h \
    Connection/serial_write_thread.h \
    Meters/Vsi/vsi.h \
    Meters/Alt/alt.h

FORMS += \
        mainwindow.ui \
    Connection/connection.ui \
    Meters/meters.ui \
    Meters/meters.ui \
    HUD/hud.ui \
    Curve/datacurve.ui \
    Progress/progress.ui \
    Map/map.ui \
    Warn/warnitem.ui \
    Warn/warning.ui \
    Compass/compasswidget.ui \
    HUD/hudview.ui \
    FlightControl/flightcontrol.ui \
    TachoMeter/tachometer.ui \
    AttitudeCtl/attitudecontrol.ui \
    ValueControl/valuecontrol.ui \
    Uav/uav.ui \
    cmdparam.ui \
    FlightControl/trackplanning.ui \
    Meters/Vsi/vsi.ui \
    Meters/Alt/alt.ui

# OSG begin config
INCLUDEPATH += E:/OSG/osg_x86/include
LIBS += -LE:/OSG/osg_x86/lib/ -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgViewerd -losgTextd
#LIBS += -LE:/OSG/osg_x86/lib/ -lOpenThreads -losg -losgDB -losgUtil -losgGA -losgViewer -losgText
# OSG end config

# ---------------Qwt begin config---------------
#DEFINES += QT_DLL QWT_DLL
#INCLUDEPATH += E:\Soft\Qt\Qt5.9.4\5.9.4\msvc2013\include\Qwt
#LIBS += -LE:\Soft\Qt\Qt5.9.4\5.9.4\msvc2013\lib -lqwtd
#LIBS += -LE:\Soft\Qt\Qt5.9.4\5.9.4\msvc2013\lib -lqwt
# ---------------Qwt end config---------------


# ---------------MAVLink V2.0---------------
INCLUDEPATH += ./mavlink_v1.0
# ---------------MAVLink V2.0---------------

RESOURCES += \
    resources/qfi.qrc \
    resources/qvumeter.qrc \
    resources/image.qrc

DISTFILES += \
    resources/qvumeter.png \
    qwebchannel.js
