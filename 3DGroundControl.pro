#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T12:46:01
#
#-------------------------------------------------

QT       += core gui svg serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DGroundControl
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
    MSProtocol/attitude_protocol_process.cpp \
    MSProtocol/msp_protocol_convert_to_real_data.cpp \
    MSProtocol/msp_protocol_process_interface.cpp \
    Connection/connection.cpp \
    Tcp/tcp_client.cpp \
    3D/qt_osg_widget.cpp \
    3D/QtOSG/OSGWidget.cpp \
    3D/QtOSG/PickHandler.cpp \
    Meters/meters.cpp \
    Meters/QMeter/qmeter.cpp \
    Meters/QMeter/qmeterdrawfunctions.cpp \
    MSProtocol/motor_protocol_process.cpp \
    QFlightInstruments/qfi_PFD.cpp \
    QFlightInstruments/qfi_ADI.cpp \
    QFlightInstruments/qfi_ALT.cpp \
    QFlightInstruments/qfi_ASI.cpp \
    QFlightInstruments/qfi_HSI.cpp \
    QFlightInstruments/qfi_NAV.cpp \
    QFlightInstruments/qfi_TC.cpp \
    QFlightInstruments/qfi_VSI.cpp \
    HUD/hud.cpp \
    MSProtocol/rc_protocol_process.cpp \
    MSProtocol/altitude_protocol_process.cpp \
    MSProtocol/analog_protocol_process.cpp \
    MSProtocol/rctuning_protocol_process.cpp \
    MSProtocol/pid_protocol_process.cpp \
    MSProtocol/rcdeadbands_protocol_process.cpp \
    MSProtocol/motorpins_protocol_process.cpp \
    Meters/QGauge/qgauge.cpp \
    Meters/QGauge/qgaugedrawfunctions.cpp \


HEADERS += \
        mainwindow.h \
    MessageBus/Any.hpp \
    MessageBus/function_traits.hpp \
    MessageBus/MessageBus.hpp \
    Communication/st_asio_wrapper.h \
    Communication/st_asio_wrapper_base.h \
    Communication/st_asio_wrapper_client.h \
    Communication/st_asio_wrapper_connector.h \
    Communication/st_asio_wrapper_object_pool.h \
    Communication/st_asio_wrapper_packer.h \
    Communication/st_asio_wrapper_server.h \
    Communication/st_asio_wrapper_server_socket.h \
    Communication/st_asio_wrapper_service_pump.h \
    Communication/st_asio_wrapper_socket.h \
    Communication/st_asio_wrapper_ssl.h \
    Communication/st_asio_wrapper_tcp_client.h \
    Communication/st_asio_wrapper_tcp_socket.h \
    Communication/st_asio_wrapper_timer.h \
    Communication/st_asio_wrapper_udp_client.h \
    Communication/st_asio_wrapper_udp_socket.h \
    Communication/st_asio_wrapper_unpacker.h \
    MSProtocol/attitude_protocol_process.h \
    MSProtocol/msp_protocol_convert_to_real_data.h \
    MSProtocol/msp_protocol_process_interface.h \
    MSProtocol/msp_protocol_command.h \
    MSProtocol/msp_protocol_structs.h \
    Connection/connection.h \
    Tcp/tcp_client.h \
    3D/qt_osg_widget.h \
    3D/QtOSG/OSGWidget.h \
    3D/QtOSG/PickHandler.h \
    Meters/meters.h \
    Meters/QMeter/qmeter.h \
    MSProtocol/motor_protocol_process.h \
    QFlightInstruments/qfi_PFD.h \
    QFlightInstruments/qfi_ADI.h \
    QFlightInstruments/qfi_ALT.h \
    QFlightInstruments/qfi_ASI.h \
    QFlightInstruments/qfi_HSI.h \
    QFlightInstruments/qfi_NAV.h \
    QFlightInstruments/qfi_TC.h \
    QFlightInstruments/qfi_VSI.h \
    HUD/hud.h \
    MSProtocol/rc_protocol_process.h \
    MSProtocol/altitude_protocol_process.h \
    MSProtocol/analog_protocol_process.h \
    MSProtocol/rctuning_protocol_process.h \
    MSProtocol/pid_protocol_process.h \
    MSProtocol/rcdeadbands_protocol_process.h \
    MSProtocol/motorpins_protocol_process.h \
    Meters/QGauge/qgauge.h \

FORMS += \
        mainwindow.ui \
    Connection/connection.ui \
    Meters/meters.ui \
    Meters/meters.ui \
    HUD/hud.ui

# Add OSG inc and lib path
INCLUDEPATH += E:/OSG/osg_x86/include
LIBS += -LE:/OSG/osg_x86/lib/ -lOpenThreadsd -losgd -losgDBd -losgUtild -losgGAd -losgViewerd -losgTextd

RESOURCES += \
    resources/qfi.qrc \


DISTFILES += \


