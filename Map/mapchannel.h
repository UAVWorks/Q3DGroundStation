#ifndef MAPCHANNEL_H
#define MAPCHANNEL_H

#include "map.h"
#include <QObject>

/*
    An instance of this class gets published over the WebChannel and is then accessible to HTML clients.
*/
class MapChannel : public QObject
{
    Q_OBJECT

public:
    MapChannel(Map *map, QObject *parent = nullptr)
        : QObject(parent), map_(map)
    {
        connect(map, &Map::SendDataToJS, this, &MapChannel::SendDataToJS);
    }

signals:
    /**
     * @brief This signal is emitted from the C++ side and handle msg on the JS client side.
     */
    void SendDataToJS(const int send_data);

public slots:

    /**
     * @brief This slot is invoked from the HTML client side and handle data on the C++ side.
     */
    void ReceiveDataFromJS(const double task_data) {
      map_->HandleJsData(task_data);
    }

private:
    Map *map_;
};


#endif // MAPCHANNEL_H
