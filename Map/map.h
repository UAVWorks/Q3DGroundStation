#ifndef MAP_H
#define MAP_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
  class Map;
}

class QWebEngineView;
class QWebSocketServer;
class WebSocketClientWrapper;
class QWebChannel;
class MapChannel;

class Map : public QWidget
{
  Q_OBJECT

public:
  explicit Map(QWidget *parent = 0);
  ~Map();


private slots:
  void RunJavaScript(bool flag);

public slots:
  void UpdateGlobalPositionInt(const mavlink_global_position_int_t &gps);
  void ReceiveData2(const double task_array[]);

signals:
  void SendDataToJS(const int send_data);

private:
  void InitWebChannel();

public:
  void HandleJsData(const double task_data);

private:
  Ui::Map *ui;
  QWebEngineView *web_view_;
  QWebSocketServer *server_;
  WebSocketClientWrapper *client_wrapper_;
  QWebChannel *channel_;
  MapChannel *map_channel_;
  double lat_;
  double lng_;
};

#endif // MAP_H
