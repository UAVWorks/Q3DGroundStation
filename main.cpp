#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <vector>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QByteArray>
#include <QPalette>
#include <QtWebEngine>
#include <QWebChannel>
#include <QWebSocketServer>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include "shared/websocketclientwrapper.h"
#include "shared/websockettransport.h"

#include "Connection/connection.h"
#include "Meters/meters.h"
#include "HUD/hud.h"
#include "Curve/data_curve.h"
#include "3D/qt_osg_widget.h"
#include "Progress/progress.h"
#include "Map/map.h"
#include "Warn/warning.h"
#include "FlightControl/flight_control.h"

#include "3D/QtOSG/OSGWidget.h"
#include "ValueControl/value_control.h"

#include <QSplashScreen>
#include <QTest>

/**
  darkGray,
  gray,
  lightGray,
 */
const QString SplitterLineColor = "QSplitter::handle {background-color: darkGray}";

void SplitterWindow(MainWindow *w);
void InitSplashWindow();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setProfile( QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QtWebEngine::initialize();

    //InitSplashWindow();

    MainWindow w;
    w.setStatusBar(nullptr);
    SplitterWindow(&w);

    w.showMaximized();

    return a.exec();
}

void InitSplashWindow() {
  QSplashScreen *splash = new QSplashScreen;
  splash->setPixmap(QPixmap(":/image/splash.png"));
  splash->show();

  Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
  splash->showMessage(QObject::tr("Setting up the main Window..."),
                    topRight,
                    Qt::red);

  QTest::qSleep(3000);
  QTextEdit *textEdit = new QTextEdit;
  splash->showMessage(QObject::tr("Loading modules..."),
                    topRight,
                    Qt::blue);

  QTest::qSleep(3000);
  textEdit->show();
  splash->finish(textEdit);
  delete splash;
}

void InitWebChannel(Map *map) {
  QFileInfo jsFileInfo("E:/Qt Project/Graduation Project/Q3DGroundStation/qwebchannel.js");

  // setup the QWebSocketServer
  QWebSocketServer *server = new QWebSocketServer(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode);
  if (!server->listen(QHostAddress::LocalHost, 12347)) {
      //qFatal("Failed to open web socket server.");
      qDebug() << "Failed to open web socket server.";
      return;
  }

  // wrap WebSocket clients in QWebChannelAbstractTransport objects
  WebSocketClientWrapper *clientWrapper = new WebSocketClientWrapper(server);

  // setup the channel
  QWebChannel *channel = new QWebChannel;
  QObject::connect(clientWrapper, &WebSocketClientWrapper::clientConnected,
                   channel, &QWebChannel::connectTo);

  // setup the core and publish it to the QWebChannel
  channel->registerObject(QStringLiteral("map"), map);
}


void SplitterWindow(MainWindow *w)
{
    QWidget *wnd = new QWidget;

    QSplitter *splitter1 = new QSplitter(Qt::Vertical);
    // 3D 模块
    QTextEdit *widget_3d2 = new QTextEdit(QObject::tr("3D"));
    //OSGWidget *widget_3d = new OSGWidget;
    QtOSGWidget *widget_3d = new QtOSGWidget(1, 1);

    QSplitter *splitter2 = new QSplitter(Qt::Horizontal);
    // 地图模块
    QTextEdit *map2 = new QTextEdit(QObject::tr("Map"));
    Map *map = new Map;

    QSplitter *splitter3 = new QSplitter(Qt::Vertical);
    // 告警模块
    QTextEdit *warn2 = new QTextEdit(QObject::tr("Warn"));
    Warning *warn = new Warning;

    QPalette pal(warn->palette());
    warn->setAutoFillBackground(true);
    pal.setColor(QPalette::Background, Qt::black);
    warn->setPalette(pal);

    // 仪表模块
    QTextEdit *meters2 = new QTextEdit(QObject::tr("Meters"));
    Meters *meters = new Meters;
    QPalette pal2(meters->palette());
    meters->setAutoFillBackground(true);
    pal2.setColor(QPalette::Background, Qt::black);
    meters->setPalette(pal2);

    splitter3->addWidget(meters);
    splitter3->addWidget(warn);
    splitter3->setStyleSheet(SplitterLineColor);
    splitter3->setStretchFactor(0, 1);
    splitter3->setStretchFactor(1, 1);

    // 存放 3D 和数值显示控件的分割窗口
    QSplitter *splitter5 = new QSplitter(Qt::Horizontal);

    QTextEdit *show_value2 = new QTextEdit(QObject::tr("show value"));
    ValueControl *show_value = new ValueControl;
    QPalette pal3(show_value->palette());
    show_value->setAutoFillBackground(true);
    pal3.setColor(QPalette::Background, Qt::black);
    show_value->setPalette(pal3);

    QSplitter *splitter4 = new QSplitter(Qt::Vertical);
    QTextEdit *hud2 = new QTextEdit(QObject::tr("Video & HUD"));
    HUD *hud = new HUD;
    QPalette pal4(hud->palette());
    hud->setAutoFillBackground(true);
    pal4.setColor(QPalette::Background, Qt::black);
    hud->setPalette(pal4);

    QTextEdit *flight_ctl2 = new QTextEdit(QObject::tr("Flight Control"));
    FlightControl *flight_ctl = new FlightControl;
    //QTextEdit *editor7 = new QTextEdit(QObject::tr("Connect"));
    Connection *connection = new Connection;
    QPalette pal5(connection->palette());
    connection->setAutoFillBackground(true);
    pal5.setColor(QPalette::Background, Qt::black);
    connection->setPalette(pal5);

    splitter5->addWidget(hud);

    splitter2->addWidget(widget_3d);
    splitter2->addWidget(splitter3);
    splitter2->setStyleSheet(SplitterLineColor);
    splitter2->setStretchFactor(0, 1);
    splitter2->setStretchFactor(1, 2);


    splitter5->addWidget(show_value);
    splitter5->setStyleSheet(SplitterLineColor);
    splitter5->setStretchFactor(0, 3);
    splitter5->setStretchFactor(1, 1);

    splitter1->addWidget(splitter5);
    splitter1->addWidget(splitter2);
    splitter1->setStyleSheet(SplitterLineColor);
    splitter1->setStretchFactor(0, 3);
    splitter1->setStretchFactor(1, 1);


    //splitter4->addWidget(editor5);
    splitter4->addWidget(map);

    splitter4->addWidget(flight_ctl);
    splitter4->addWidget(connection);
    splitter4->setStyleSheet(SplitterLineColor);
    splitter4->setStretchFactor(0, 3);
    splitter4->setStretchFactor(1, 1);
    splitter4->setStretchFactor(2, 1);

    QSplitter *splitter_main = new QSplitter(Qt::Horizontal);
    splitter_main->addWidget(splitter1);
    splitter_main->addWidget(splitter4);
    splitter_main->setStyleSheet(SplitterLineColor);
    splitter_main->setStretchFactor(0, 5);
    splitter_main->setStretchFactor(1, 2);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter_main);

    // 加上 Widget 有边界
    wnd->setLayout(layout);
    w->setCentralWidget(wnd);

    // 直接添加没有边界
    //main_window.setCentralWidget(splitter_main);

    // 拖拽分割条时，是否实时更新
    //splitter_right->setOpaqueResize(true);

    // must be public slot!

    QObject::connect(connection, &Connection::UpdateGlobalPositionInt, map, &Map::UpdateGlobalPositionInt);

    QObject::connect(connection, &Connection::UpdateAttitude, hud, &HUD::UpdateAttitude);
    QObject::connect(connection, &Connection::UpdateHUD, hud, &HUD::UpdateHUD);
    QObject::connect(connection, &Connection::UpdateGPSStatus, hud, &HUD::UpdateGPSStatus);

    QObject::connect(connection, &Connection::UpdateHUD, meters, &Meters::UpdateHUD);
    QObject::connect(connection, &Connection::UpdateAlt, meters, &Meters::UpdateAlt);

    QObject::connect(connection, &Connection::UpdateAttitude, widget_3d, &QtOSGWidget::Update3D);

    QObject::connect(connection, &Connection::UpdateAttitude, meters, &Meters::UpdateAttitude);
    QObject::connect(connection, &Connection::UpdateSysStatusSensor, warn, &Warning::UpdateSysStatusSensor);
    QObject::connect(connection, &Connection::UpdateAirSpeed, show_value, &ValueControl::UpdateAirSpeed);
    QObject::connect(connection, &Connection::UpdateGroundSpeed, show_value, &ValueControl::UpdateGroundSpeed);
    QObject::connect(connection, &Connection::UpdateMavMode, show_value, &ValueControl::UpdateFlightMode);
    QObject::connect(connection, &Connection::UpdateARM, show_value, &ValueControl::UpdateARM);
    QObject::connect(connection, &Connection::UpdateAttitude, show_value, &ValueControl::UpdateAttitude);
    QObject::connect(connection, &Connection::UpdateGlobalPositionInt, show_value, &ValueControl::UpdateGlobalPositionInt);
    QObject::connect(connection, &Connection::UpdateServoOutput, meters, &Meters::UpdateServoOutput);
    QObject::connect(connection, &Connection::UpdateServoOutput, flight_ctl, &FlightControl::UpdateServoOutput);

    QObject::connect(flight_ctl, &FlightControl::SendMAVLinkCmd, connection, &Connection::SendMAVLinkCmd);
    QObject::connect(flight_ctl, &FlightControl::SendRcChannels, connection, &Connection::SendRcChannels);
    QObject::connect(flight_ctl, &FlightControl::SendUnlockSignal, connection, &Connection::SendUnlockSignal);
}
