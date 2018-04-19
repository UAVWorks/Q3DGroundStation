#include "map.h"
#include "ui_map.h"

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QMessageBox>
#include <QWebChannel>
#include <QWebSocketServer>
#include <QDir>
#include <QFileInfo>

#include "shared/websocketclientwrapper.h"
#include "shared/websockettransport.h"

#include "mapchannel.h"

Map::Map(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Map),
  server_(nullptr),
  client_wrapper_(nullptr),
  channel_(nullptr),
  map_channel_(nullptr),
  lat_(0.0),
  lng_(0.0)
{
  ui->setupUi(this);

  web_view_ = new QWebEngineView(this);
  web_view_->load(QUrl("E:/Qt Project/Graduation Project/Q3DGroundStation/Q3DGS_Google_Map.html"));
  //web_view_->load(QUrl("E:/Q3DGS/GoogleMapAPIV3/Example_GoogleMapAPI.html"));
  ui->mapLayout->addWidget(web_view_);

  InitWebChannel();

  connect(web_view_->page(), &QWebEnginePage::loadFinished, this, &Map::RunJavaScript);
}

Map::~Map()
{
  delete ui;
}

/**
 * @brief Init web channel
 * @note WebSocket: 127.0.0.1 : 12345
 */
void Map::InitWebChannel() {
  //QFileInfo jsFileInfo("E:/Qt Project/Graduation Project/Q3DGroundStation/qwebchannel.js");

  // setup the QWebSocketServer
  server_ = new QWebSocketServer(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode);
  if (!server_->listen(QHostAddress::LocalHost, 12345)) {
      //qFatal("Failed to open web socket server.");
      qDebug() << "Failed to open web socket server.";
      return;
  }

  // wrap WebSocket clients in QWebChannelAbstractTransport objects
  client_wrapper_ = new WebSocketClientWrapper(server_);

  // setup the channel
  channel_ = new QWebChannel;

  QObject::connect(client_wrapper_, &WebSocketClientWrapper::clientConnected,
                   channel_, &QWebChannel::connectTo);

  map_channel_ = new MapChannel(this);
  // setup the core and publish it to the QWebChannel
  channel_->registerObject(QStringLiteral("mapwidget"), map_channel_);
}

/**
 * @brief Run Java Script function
 * @note NCHU(28.654752, 115.824807)
 */
void Map::RunJavaScript(bool flag) {
  QString js_fun = QString("locate(%1, %2);").arg(28.654752).arg(115.824807);
  web_view_->page()->runJavaScript(js_fun);
}


/**
 * @brief Update map uav marker
 */
void Map::UpdateGPS(const mavlink_gps_raw_int_t &gps) {
  lat_ = gps.lat;
  lng_ = gps.lon;

  // 定位 + 绘制路径
  QString js_fun = QString("locate(%1, %2);").arg(lat_).arg(lng_);
  web_view_->page()->runJavaScript(js_fun);
}

/**
 * @brief Receiver task data from JS
 * @note No find way to let JS send taskArray to Qt
 */
void Map::ReceiveData2(const double task_array[]) {
  QMessageBox::information(NULL, "Map Info", QString::number(task_array[0]), QMessageBox::Yes);
  QMessageBox::information(NULL, "Map Info", QString::number(task_array[1]), QMessageBox::Yes);
}

/**
 * @brief Handle JS task data
 * @note task_data: lat1, lng1, lat2, lng2, lat3, lng3 ...... latn, lngn
 */
void Map::HandleJsData(const double task_data) {
  QMessageBox::information(NULL, "Map Info", QString::number(task_data), QMessageBox::Yes);

  // Call MAVLinkManager send task to UAV
}
