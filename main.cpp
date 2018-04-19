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

void SplitterWindow(MainWindow *w);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
*/
    QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setProfile( QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QtWebEngine::initialize();

    MainWindow w;
    SplitterWindow(&w);

    w.showMaximized();

    return a.exec();
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

    splitter3->addWidget(warn);
    splitter3->addWidget(meters);

    splitter2->addWidget(map);
    splitter2->addWidget(splitter3);

    splitter1->addWidget(widget_3d);
    splitter1->addWidget(splitter2);

    splitter1->setStretchFactor(0, 2);
    splitter1->setStretchFactor(1, 1);

    QSplitter *splitter4 = new QSplitter(Qt::Vertical);
    QTextEdit *hud2 = new QTextEdit(QObject::tr("Video & HUD"));
    HUD *hud = new HUD;

    QTextEdit *flight_ctl2 = new QTextEdit(QObject::tr("Flight Control"));
    FlightControl *flight_ctl = new FlightControl;
    //QTextEdit *editor7 = new QTextEdit(QObject::tr("Connect"));
    Connection *connection = new Connection;
    //splitter4->addWidget(editor5);
    splitter4->addWidget(hud);

    splitter4->addWidget(flight_ctl);
    splitter4->addWidget(connection);
    splitter4->setStretchFactor(0, 2);
    splitter4->setStretchFactor(1, 1);
    splitter4->setStretchFactor(2, 1);

    QSplitter *splitter_main = new QSplitter(Qt::Horizontal);
    splitter_main->addWidget(splitter1);
    splitter_main->addWidget(splitter4);
    splitter_main->setStretchFactor(0, 3);
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
    QObject::connect(connection, &Connection::UpdateGPS, meters, &Meters::UpdateGPS);
    QObject::connect(connection, &Connection::UpdateGPS, map, &Map::UpdateGPS);
    QObject::connect(connection, &Connection::UpdateAttitude, hud, &HUD::UpdateHUD);
    QObject::connect(connection, &Connection::UpdateAttitude, widget_3d, &QtOSGWidget::Update3D);
    QObject::connect(connection, &Connection::UpdateAirSpeed, meters, &Meters::UpdateAirSpeed);
    QObject::connect(connection, &Connection::UpdateAttitude, meters, &Meters::UpdateAttitude);
    QObject::connect(connection, &Connection::UpdateSysStatusSensor, warn, &Warning::UpdateSysStatusSensor);

    //InitWebChannel(map);
}
